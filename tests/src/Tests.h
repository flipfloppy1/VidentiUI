#pragma once

#include "VidentiUI.h"
#include "VidentiParse.h"
#include "VidentiAngleRenderer.h"
#define WINDOW_WIDTH 2560
#define WINDOW_HEIGHT 1440

namespace VUI
{
	namespace Tests
	{
		bool LoadParseTest(VidentiHandler& uiHandler);
		bool UnloadParseTest(VidentiHandler& uiHandler);
		bool LoadRendererTest(VidentiHandler& uiHandler);
		bool UnloadRendererTest(VidentiHandler& uiHandler);
	}
}