#include "VidentiUI.h"
#include "Parse.h"
#include <nlohmann/json.h>
#include <fstream>
#include <iostream>
#include <string>

void VUI::UIHandler::Render()
{

}

void VUI::UIHandler::StartFrame()
{

}

void VUI::UIHandler::EndFrame()
{

}

void VUI::UIHandler::ParseUI(const char* filepath)
{
	std::ifstream f;
	f.open(filepath);
	
	if (!f.is_open())
	{
		VUI::Log(ERROR_MAJOR, "Unable to find file, returning with empty UI");
		elements.clear();
		return;
	}

	nlohmann::json uiObject = nlohmann::json::parse(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>(0));
	
	f.close();

	if (!uiObject.contains("ui"))
	{
		VUI::Log(ERROR_MINOR, "UI json empty, returning with empty UI");
		elements.clear();
		return;
	}

	std::vector<UIElement*> newElements;

	newElements = ParseObjects(uiObject, "buttons", ParseButton);
	elements.insert(elements.end(), newElements.begin(), newElements.end());
	newElements = ParseObjects(uiObject, "text", ParseText);
	elements.insert(elements.end(), newElements.begin(), newElements.end());
	newElements = ParseObjects(uiObject, "rectangles", ParseRectangle);
	elements.insert(elements.end(), newElements.begin(), newElements.end());
	newElements = ParseObjects(uiObject, "sliders", ParseSlider);
	elements.insert(elements.end(), newElements.begin(), newElements.end());
	newElements = ParseObjects(uiObject, "textures", ParseTexture);
	elements.insert(elements.end(), newElements.begin(), newElements.end());
}

void VUI::Log(ErrorCode errorCode, const char* message)
{
	std::string errorString;

	switch (errorCode)
	{
	case ERROR_NONE:
		errorString = "None";
		break;
	case ERROR_MINOR:
		errorString = "Minor";
		break;
	case ERROR_MAJOR:
		errorString = "Major";
		break;
	case ERROR_FATAL:
		errorString = "FATAL";
		break;
	}

#ifndef VIDENTI_SILENT
	std::cout << "Logging with error code: " << errorString << "; message: " << message << '\n';
#else
	isMessage = true;
	currMessage = message;
	currErrorCode = errorCode;
#endif
}

VUI::Message VUI::FetchMessage()
{
	if (!isMessage)
		return { "No message found.", ERROR_NONE };

	isMessage = false;
	return { currMessage,currErrorCode };
}