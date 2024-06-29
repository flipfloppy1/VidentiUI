
#include "Tests.h"

bool VUI::Tests::LoadParseTest(UIHandler& uiHandler)
{
	uiHandler.ParseUI("resources/parseTest.json");
	
	// Test element parsing
	if (uiHandler.FindElement("button1") == nullptr)
		return false;

	// Test default values
	if (!(uiHandler.FindElement("button1")->color == std::any_cast<VUI::Math::u8vec4>(VUI::propertyDefaults.at("color"))))
		return false;

	return true;
}

bool VUI::Tests::UnloadParseTest(UIHandler& uiHandler)
{
	return true;
}