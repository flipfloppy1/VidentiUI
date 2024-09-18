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
			};
			GLFWCallbackHandler* glfwCallbackPointer;
		private:
			GLFWwindow* window;
		};
	}
}
