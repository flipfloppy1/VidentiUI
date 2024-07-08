#pragma once

#include <map>
#include <mutex>

namespace VUI
{
	namespace Poller
	{
		struct KeyState
		{
			bool pressCurr;
			bool pressPrev;
			bool releaseCurr;
			bool releasePrev;
		};

		typedef std::map<int, KeyState> KeyMap;

		class VidentiPoller
		{
		public:
			virtual void RefreshEvents() = 0;
			virtual KeyMap GetKeyStates() = 0;
			virtual void Init() = 0;
		protected:
			KeyMap keyMap;
			std::mutex keyMapMutex;
		};
	}
}
