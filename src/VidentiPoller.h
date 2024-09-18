#pragma once

#include <map>
#include <mutex>
#include <atomic>

namespace VUI
{
	namespace Poller
	{
		struct KeyState
		{
			bool pressCurr;
			bool pressPrev;
		};

		struct MouseState
		{
			double mouseX = 0.0;
			double mouseY = 0.0;
			bool mouseDownCurr = false;
			bool mouseDownPrev = false;
		};

		typedef std::map<int, KeyState> KeyMap;

		class VidentiPoller
		{
		public:
			virtual void RefreshEvents() = 0;
			virtual KeyMap GetKeyStates() = 0;
			virtual void Init() = 0;
			virtual MouseState GetMouseState() = 0;
		protected:
			KeyMap keyMap;
			MouseState mouseState;
			std::mutex keyMapMutex;
		};
	}
}
