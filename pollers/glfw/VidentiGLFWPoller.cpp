
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
		state.pressPrev = state.pressCurr;

	keyMapMutex.unlock();
	mouseState.mouseDownPrev = mouseState.mouseDownCurr;

	glfwPollEvents();

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	mouseState.mouseX = x;
	mouseState.mouseY = y;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		mouseState.mouseDownCurr = true;
	else
		mouseState.mouseDownCurr = false;
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
		keyState.pressCurr = true;
	else
		keyState.pressCurr = false;

	uiPoller->keyMapMutex.unlock();
}

VUI::Poller::MouseState VUI::Poller::VidentiGLFWPoller::GetMouseState()
{
	return mouseState;
}
