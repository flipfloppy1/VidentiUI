
#include "Parse.h"

void VUI::ParseObjects(nlohmann::json uiObject, std::string typeString, void (*parseFunction)(nlohmann::json))
{
	if (!uiObject.at("ui").contains(typeString))
		return;
	else if (!uiObject.at("ui").at(typeString).is_array())
	{
		std::string errorString = "Malformed json: \"" + typeString + "\" was not an array";
		VUI::Log(ERROR_MINOR, errorString.c_str());
		return;
	}

	bool parseError = false;
	for (nlohmann::json element : uiObject.at("ui").at(typeString))
	{
		parseFunction(element);
	}
}

bool VUI::HasValidInt(nlohmann::json element, std::string propertyName)
{
	if (element.contains(propertyName))
	{
		return element.at(propertyName).is_number_integer();
	}
	return false;
}

bool VUI::HasValidVec2(nlohmann::json element, std::string propertyName)
{
	if (element.contains(propertyName))
	{
		if (element.at(propertyName).is_array())
		{
			if (element.at(propertyName).size() == 2)
			{
				return element.at(propertyName)[0].is_number() && element.at(propertyName)[1].is_number();
			}
		}
	}
	return false;
}

bool VUI::HasValidColor(nlohmann::json element, std::string propertyName)
{
	if (element.contains(propertyName))
	{
		if (element.at(propertyName).is_array())
		{
			if (element.at(propertyName).size() == 4)
			{
				bool valid = true;
				for (int i = 0; i < 4; i++)
				{
					if (!element.at(propertyName)[i].is_number_integer())
					{
						valid = false;
						break;
					}
					valid &= element.at(propertyName)[i] >= 0 && element.at(propertyName)[i] <= 255;
				}
				return valid;
			}
		}
	}
	return false;
}

VUI::Math::u8vec4 inline VUI::GetColor(nlohmann::json colorProperty)
{
	return Math::u8vec4{ (unsigned char)colorProperty[0], (unsigned char)colorProperty[1], (unsigned char)colorProperty[2], (unsigned char)colorProperty[3] };
}

VUI::Math::vec2 inline VUI::GetVec2(nlohmann::json vecProperty)
{
	return { (double)vecProperty[0],(double)vecProperty[1] };
}

void VUI::ParseGenericProperties(nlohmann::json element, VUI::UIElement& newElement)
{
	if (HasValidVec2(element, "position"))
		newElement.position = GetVec2(element.at("position"));
	else
		newElement.position = Math::vec2(0.0);
	
	if (HasValidVec2(element, "dimensions"))
		newElement.dimensions = GetVec2(element.at("dimensions"));
	else
		newElement.dimensions = Math::vec2(1.0);
	
	if (HasValidColor(element, "color"))
		newElement.color = GetColor(element.at("color"));
	else
		newElement.color = Math::u8vec4(255);

	if (HasValidInt(element, "layer"))
		newElement.layer = element.at("layer");
	else
		newElement.layer = 0;
}

void VUI::ParseRectangle(nlohmann::json element)
{
	UIElement newElement{};
	ParseGenericProperties(element, newElement);
}

void VUI::ParseTexture(nlohmann::json element)
{
	UIElement newElement{};
	ParseGenericProperties(element, newElement);
}

void VUI::ParseButton(nlohmann::json element)
{
	UIElement newElement{};
	ParseGenericProperties(element, newElement);
}

void VUI::ParseText(nlohmann::json element)
{
	UIElement newElement{};
	ParseGenericProperties(element, newElement);
}

void VUI::ParseSlider(nlohmann::json element)
{
	UIElement newElement{};
	ParseGenericProperties(element, newElement);
}