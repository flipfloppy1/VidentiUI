
#include "VidentiParse.h"

std::vector<VUI::UIElement*> VUI::ParseObjects(nlohmann::json uiObject, std::string typeString, VUI::UIElement* (*parseFunction)(nlohmann::json))
{
	std::vector<UIElement*> elements;
	if (!uiObject.at("ui").contains(typeString))
		return std::vector<UIElement*>();
	else if (!uiObject.at("ui").at(typeString).is_array())
	{
		std::string errorString = "Malformed json: \"" + typeString + "\" was not an array";
		VUI::Log(VUI::ERROR_MINOR, errorString.c_str());
		return std::vector<UIElement*>();
	}

	bool parseError = false;
	for (nlohmann::json element : uiObject.at("ui").at(typeString))
	{
		elements.push_back(parseFunction(element));
	}
	return elements;
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

bool VUI::HasValidVec2Bool(nlohmann::json element, std::string propertyName)
{
	if (element.contains(propertyName))
	{
		if (element.at(propertyName).is_array())
		{
			if (element.at(propertyName).size() == 2)
			{
				return element.at(propertyName)[0].is_boolean() && element.at(propertyName)[1].is_boolean();
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
	return { vecProperty[0], vecProperty[1] };
}

void VUI::ParseGenericProperties(nlohmann::json element, VUI::UIElement* newElement)
{
	if (element.contains("id"))
	{
		if (element.at("id").is_string())
			newElement->id = element.at("id");
		else
			newElement->id = "element" + std::to_string(rand());
	}
	else
		newElement->id = "element" + std::to_string(rand());

	if (element.contains("texture"))
	{
		if (element.at("texture").is_string())
			newElement->texture = element.at("texture");
		else
			newElement->texture = "";
	}
	else
		newElement->texture = "";

	if (HasValidVec2(element, "position"))
		newElement->position = GetVec2(element.at("position"));
	else
		newElement->position = std::any_cast<Math::vec2>(propertyDefaults.at("position"));

	if (HasValidVec2(element, "dimensions"))
		newElement->dimensions = GetVec2(element.at("dimensions"));
	else
		newElement->dimensions = std::any_cast<Math::vec2>(propertyDefaults.at("dimensions"));

	if (HasValidColor(element, "color"))
		newElement->color = GetColor(element.at("color"));
	else
		newElement->color = std::any_cast<Math::u8vec4>(propertyDefaults.at("color"));

	if (HasValidInt(element, "layer"))
		newElement->layer = element.at("layer");
	else
		newElement->layer = std::any_cast<int32_t>(propertyDefaults.at("layer"));

	if (HasValidVec2Bool(element, "ratioTransform"))
	{
		newElement->ratioTransform[0] = element.at("ratioTransform")[0];
		newElement->ratioTransform[1] = element.at("ratioTransform")[1];
	}
	else
	{
		newElement->ratioTransform[0] = std::any_cast<bool>(propertyDefaults.at("ratioTransform"));
		newElement->ratioTransform[1] = std::any_cast<bool>(propertyDefaults.at("ratioTransform"));
	}
}

VUI::UIElement* VUI::ParseRectangle(nlohmann::json element)
{
	UIElement* newElement = new Rectangle();
	ParseGenericProperties(element, newElement);
	return newElement;
}

VUI::UIElement* VUI::ParseTexture(nlohmann::json element)
{
	UIElement* newElement = new Texture();
	ParseGenericProperties(element, newElement);
	return newElement;
}

VUI::UIElement* VUI::ParseButton(nlohmann::json element)
{
	UIElement* newElement = new Button();
	ParseGenericProperties(element, newElement);
	return newElement;
}

VUI::UIElement* VUI::ParseText(nlohmann::json element)
{
	UIElement* newElement = new Text();
	ParseGenericProperties(element, newElement);
	return newElement;
}

VUI::UIElement* VUI::ParseSlider(nlohmann::json element)
{
	UIElement* newElement = new Slider();
	ParseGenericProperties(element, newElement);
	return newElement;
}