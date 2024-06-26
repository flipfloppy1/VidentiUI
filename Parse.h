#pragma once

#include "nlohmann/json.hpp"
#include "VidentiUI.h"

namespace VUI
{
	bool HasValidVec2(nlohmann::json element, std::string propertyName);
	bool HasValidColor(nlohmann::json element, std::string propertyName);
	bool HasValidInt(nlohmann::json element, std::string propertyName);

	Math::u8vec4 inline GetColor(nlohmann::json colorProperty);
	Math::vec2 inline GetVec2(nlohmann::json vecProperty);

	void ParseObjects(nlohmann::json uiObject, std::string typeString, void (*parseFunction)(nlohmann::json));
	void ParseGenericProperties(nlohmann::json element, UIElement& newElement);
	void ParseRectangle(nlohmann::json element);
	void ParseTexture(nlohmann::json element);
	void ParseButton(nlohmann::json element);
	void ParseText(nlohmann::json element);
	void ParseSlider(nlohmann::json element);
}