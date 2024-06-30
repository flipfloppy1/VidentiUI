
#include "VidentiUI.h"
#include "VidentiParse.h"
#include <nlohmann/json.h>
#include <fstream>
#include <iostream>
#include <string>

void VUI::VidentiHandler::Render()
{
	if (!uiRenderer->IsGenerated())
		uiRenderer->GenElements(elements);

	uiRenderer->Render();

}

void VUI::VidentiHandler::StartFrame()
{

}

void VUI::VidentiHandler::EndFrame()
{

}

void VUI::VidentiHandler::ParseUI(const char* filepath)
{
	std::ifstream f;
	f.open(filepath);

	if (!f.is_open())
	{
		VUI::Log(ERROR_MAJOR, "ParseUI: Unable to find file, returning with empty UI");
		elements.clear();
		return;
	}

	nlohmann::json uiObject = nlohmann::json::parse(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>(0));

	f.close();

	if (!uiObject.contains("ui"))
	{
		VUI::Log(ERROR_MINOR, "ParseUI: UI json empty, returning with empty UI");
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

	for (UIElement* element : elements)
	{
		element->uiHandler = this;
	}
}

void VUI::VidentiHandler::GenUI()
{
	if (elements.empty())
	{
		Log(VUI::ERROR_MINOR, "VidentiHandler::GenUI: No elements to generate, call ParseUI to add elements");
		return;
	}

	if (uiRenderer == nullptr)
	{
		Log(VUI::ERROR_MAJOR, "VidentiHandler::GenUI: UI Renderer has not been defined, attach using AttachRenderer");
		return;
	}

	uiRenderer->InstructGeneration();
}

void VUI::VidentiHandler::AttachRenderer(VUI::Renderer::VidentiRenderer* renderer)
{
	VidentiHandler::uiRenderer = renderer;
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

std::vector<VUI::Renderer::UIVertex> VUI::Button::GenVerts()
{
	std::vector<Renderer::UIVertex> vertices;
	Math::vec2 windowDimensions = uiHandler->GetWindowDimensions();
	Math::vec2 transformDimensions(0.0f);
	if (ratioTransform[0])
		transformDimensions.x = dimensions.x * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.x = dimensions.x;
	if (ratioTransform[1])
		transformDimensions.y = dimensions.y * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.y = dimensions.y;

	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f) }, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f + transformDimensions.y * 2.0f) }, color, { 0.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + transformDimensions.x * 2.0f, -(position.y * 2.0f - 1.0f + transformDimensions.y * 2.0f) }, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + transformDimensions.x * 2.0f, -(position.y * 2.0f - 1.0f + transformDimensions.y * 2.0f) }, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + transformDimensions.x * 2.0f, -(position.y * 2.0f - 1.0f) }, color, { 1.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f) }, color, { 0.0f,1.0f }));
	return vertices;
}

std::vector<VUI::Renderer::UIVertex> VUI::Slider::GenVerts()
{
	std::vector<Renderer::UIVertex> vertices;
	Math::vec2 windowDimensions = uiHandler->GetWindowDimensions();
	Math::vec2 transformDimensions(0.0f);
	if (ratioTransform[0])
		transformDimensions.x = dimensions.x * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.x = dimensions.x;
	if (ratioTransform[1])
		transformDimensions.y = dimensions.y * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.y = dimensions.y;

	vertices.push_back(Renderer::UIVertex(position * 2.0f - 1.0f, color, { 0.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y } *2.0f - 1.0f, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y } *2.0f - 1.0f, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 1.0f,1.0f }));
	return vertices;
}

std::vector<VUI::Renderer::UIVertex> VUI::Texture::GenVerts()
{
	std::vector<Renderer::UIVertex> vertices;
	Math::vec2 windowDimensions = uiHandler->GetWindowDimensions();
	Math::vec2 transformDimensions(0.0f);
	if (ratioTransform[0])
		transformDimensions.x = dimensions.x * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.x = dimensions.x;
	if (ratioTransform[1])
		transformDimensions.y = dimensions.y * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.y = dimensions.y;

	vertices.push_back(Renderer::UIVertex(position * 2.0f - 1.0f, color, { 0.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y } *2.0f - 1.0f, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y } *2.0f - 1.0f, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 1.0f,1.0f }));
	return vertices;
}

std::vector<VUI::Renderer::UIVertex> VUI::Rectangle::GenVerts()
{
	std::vector<Renderer::UIVertex> vertices;
	Math::vec2 windowDimensions = uiHandler->GetWindowDimensions();
	Math::vec2 transformDimensions(0.0f);
	if (ratioTransform[0])
		transformDimensions.x = dimensions.x * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.x = dimensions.x;
	if (ratioTransform[1])
		transformDimensions.y = dimensions.y * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.y = dimensions.y;

	vertices.push_back(Renderer::UIVertex(position * 2.0f - 1.0f, color, { 0.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y } *2.0f - 1.0f, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y } *2.0f - 1.0f, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 1.0f,1.0f }));
	return vertices;
}

std::vector<VUI::Renderer::UIVertex> VUI::Text::GenVerts()
{
	std::vector<Renderer::UIVertex> vertices;
	Math::vec2 windowDimensions = uiHandler->GetWindowDimensions();
	Math::vec2 transformDimensions(0.0f);
	if (ratioTransform[0])
		transformDimensions.x = dimensions.x * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.x = dimensions.x;
	if (ratioTransform[1])
		transformDimensions.y = dimensions.y * windowDimensions.x / windowDimensions.y;
	else
		transformDimensions.y = dimensions.y;

	vertices.push_back(Renderer::UIVertex(position * 2.0f - 1.0f, color, { 0.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y } *2.0f - 1.0f, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y } *2.0f - 1.0f, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x + transformDimensions.x / 2.0f,position.y + transformDimensions.y / 2.0f } *2.0f - 1.0f, color, { 1.0f,1.0f }));
	return vertices;
}