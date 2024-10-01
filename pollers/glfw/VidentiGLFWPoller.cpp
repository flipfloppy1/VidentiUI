
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
	for (char c = 32; c < 126; c++)
	{
	    keyMap[std::string(1, c)] = {};
	}
	for (auto& [key, keyStr] : glfwCallbackPointer->nonPrintableKeys)
	{
	    keyMap[keyStr] = {};
	}
}

void VUI::Poller::VidentiGLFWPoller::RefreshEvents()
{
	while (!keyMapMutex.try_lock()) {} // Wait
	for (auto& [key, state] : keyMap)
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
	KeyState* keyState = nullptr;
	bool ignored = true;
	if (glfwGetKeyName(key,scancode) != nullptr)
	{
	    ignored = false;
        std::string keyStr = glfwGetKeyName(key,scancode);
		if (keyStr.size() == 1 && keyStr.at(0) >= 97 && keyStr.at(0) <= 122)
		    uiPoller->keyMap[std::string(1, keyStr.at(0)-32)].pressCurr = action == GLFW_PRESS || action == GLFW_REPEAT;

    	keyState = &uiPoller->keyMap[keyStr];
	}
	else
	{
	    for (auto& [npKey, keyStr] : nonPrintableKeys)
		{
		    if (key == npKey)
			{
			    ignored = false;
			    keyState = &uiPoller->keyMap[keyStr];
			}
		}
	}

	if (!ignored)
  		keyState->pressCurr = action == GLFW_PRESS || action == GLFW_REPEAT;

	uiPoller->keyMapMutex.unlock();
}

VUI::Poller::MouseState VUI::Poller::VidentiGLFWPoller::GetMouseState()
{
	return mouseState;
}
