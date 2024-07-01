#pragma once

#include <nlohmann/json.hpp>
#include "VidentiUI.h"
#include "VidentiMath.h"

namespace VUI
{
	bool HasValidVec2(nlohmann::json element, std::string propertyName);
	bool HasValidVec2Bool(nlohmann::json element, std::string propertyName);
	bool HasValidColor(nlohmann::json element, std::string propertyName);
	bool HasValidInt(nlohmann::json element, std::string propertyName);

	Math::u8vec4 inline GetColor(nlohmann::json colorProperty);
	Math::vec2 inline GetVec2(nlohmann::json vecProperty);

	std::vector<UIElement*> ParseObjects(nlohmann::json uiObject, std::string typeString, UIElement* (*parseFunction)(nlohmann::json));
	void ParseGenericProperties(nlohmann::json element, UIElement* newElement);
	UIElement* ParseRectangle(nlohmann::json element);
	UIElement* ParseTexture(nlohmann::json element);
	UIElement* ParseButton(nlohmann::json element);
	UIElement* ParseText(nlohmann::json element);
	UIElement* ParseSlider(nlohmann::json element);
}