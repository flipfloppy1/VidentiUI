#pragma once

#include "VidentiPoller.h"
#include <GLFW/glfw3.h>

namespace VUI
{
	namespace Poller
	{
		class VidentiGLFWPoller : public VUI::Poller::VidentiPoller
		{
		public:
			virtual void RefreshEvents();
			virtual KeyMap GetKeyStates();
			virtual MouseState GetMouseState();
			virtual void Init();
			void AttachGLFW(GLFWwindow* window);
			class GLFWCallbackHandler
			{
			public:
				void MapKey(GLFWwindow* window, int key, int scancode, int action, int mods);
				VidentiGLFWPoller* uiPoller;

				const std::map<int,std::string> nonPrintableKeys =
                {
                    {GLFW_KEY_SPACE, "Space"},
                    {GLFW_KEY_ENTER, "Enter"},
                    {GLFW_KEY_BACKSPACE, "Backspace"},
                    {GLFW_KEY_DELETE, "Delete"},
                    {GLFW_KEY_ESCAPE, "Escape"},
                    {GLFW_KEY_TAB, "Tab"},
                    {GLFW_KEY_LEFT_SUPER, "Super"},
                    {GLFW_KEY_LEFT_ALT, "Alt"},
                    {GLFW_KEY_LEFT_CONTROL, "Ctrl"},
                    {GLFW_KEY_LEFT_SHIFT, "Shift"}
                };
			};
			GLFWCallbackHandler* glfwCallbackPointer;
		private:
			GLFWwindow* window;
		};
	}
}
