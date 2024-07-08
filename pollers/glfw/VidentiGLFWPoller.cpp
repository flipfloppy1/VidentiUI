
#include "VidentiGLFWPoller.h"

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	VUI::Poller::VidentiGLFWPoller::GLFWCallbackHandler* handler = (VUI::Poller::VidentiGLFWPoller::GLFWCallbackHandler*)glfwGetWindowUserPointer(window);
	handler->MapKey(window, key, scancode, action, mods);
}

void VUI::Poller::VidentiGLFWPoller::AttachGLFW(GLFWwindow* window)
{
	VidentiGLFWPoller::window = window;
	glfwSetKeyCallback(window, KeyCallback);
}

void VUI::Poller::VidentiGLFWPoller::Init()
{
	glfwSetWindowUserPointer(window, new VUI::Poller::VidentiGLFWPoller::GLFWCallbackHandler);
	glfwCallbackPointer = (VUI::Poller::VidentiGLFWPoller::GLFWCallbackHandler*)glfwGetWindowUserPointer(window);
	glfwCallbackPointer->uiPoller = this;
}

void VUI::Poller::VidentiGLFWPoller::RefreshEvents()
{
	while (!keyMapMutex.try_lock()) {} // Wait
	for (auto [key, state] : keyMap)
	{
		state.pressPrev = state.pressCurr;
		state.releaseCurr = state.releasePrev;
	}
	keyMapMutex.unlock();

	glfwPollEvents();
}

VUI::Poller::KeyMap VUI::Poller::VidentiGLFWPoller::GetKeyStates()
{
	std::unique_lock<std::mutex> lock(keyMapMutex);
	return keyMap;
}

void VUI::Poller::VidentiGLFWPoller::GLFWCallbackHandler::MapKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	while (!uiPoller->keyMapMutex.try_lock()) {} // Wait
	KeyState& keyState = uiPoller->keyMap[key];

	if (action == GLFW_PRESS)
	{
		keyState.pressCurr = true;
	}

	if (action == GLFW_RELEASE)
	{
		keyState.releaseCurr = true;
	}
	uiPoller->keyMapMutex.unlock();
}
