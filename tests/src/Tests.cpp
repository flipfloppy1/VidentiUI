
#include "Tests.h"

bool VUI::Tests::LoadRendererTest(VidentiHandler& uiHandler)
{
	uiHandler.AttachRenderer(new VUI::Renderer::VidentiAngleRenderer());
	uiHandler.InitRenderer({ 2560,1440 });
	return true;
}

bool VUI::Tests::UnloadRendererTest(VidentiHandler& uiHandler)
{
	return true;
}

bool VUI::Tests::LoadParseTest(VidentiHandler& uiHandler)
{
	bool testResult = true;

	uiHandler.ParseUI("resources/parseTest.json");

	// Test element parsing
	if (uiHandler.FindElement("button1") == nullptr)
		testResult = false;

	// Test default properties
	if (!(uiHandler.FindElement("button1")->color == std::any_cast<VUI::Math::u8vec4>(VUI::propertyDefaults.at("color"))))
		testResult = false;

	// Test parsed properties
	if (!(uiHandler.FindElement("button1")->position == Math::vec2(0.45f)))
		testResult = false;

	uiHandler.GenUI();

	return testResult;
}

bool VUI::Tests::UnloadParseTest(VidentiHandler& uiHandler)
{
	return true;
}