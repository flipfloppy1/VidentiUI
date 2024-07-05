
#include "Tests.h"

bool VUI::Tests::LoadRendererTest(VidentiHandler& uiHandler)
{
	uiHandler.AttachRenderer(new VUI::Renderer::VidentiAngleRenderer(), { WINDOW_WIDTH,WINDOW_HEIGHT });
	uiHandler.Init();
	return true;
}

bool VUI::Tests::UnloadRendererTest(VidentiHandler& uiHandler)
{
	return true;
}

bool VUI::Tests::LoadParseTest(VidentiHandler& uiHandler)
{
	bool testResult = true;

	uiHandler.ParseUI("resources/start.lua");

	// Test element parsing
	if (uiHandler.FindElement("button1") == nullptr)
		testResult = false;

	uiHandler.GenUI();

	return testResult;
}

bool VUI::Tests::UnloadParseTest(VidentiHandler& uiHandler)
{
	return true;
}