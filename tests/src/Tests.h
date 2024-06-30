#pragma once

#include "VidentiUI.h"
#include "VidentiParse.h"
#include "VidentiAngleRenderer.h"

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