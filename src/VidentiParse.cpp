
#include "VidentiParse.h"

std::map<std::string, VUI::UIElement*> VUI::ParseObjects(lua_State* lua, std::string typeString)
{
	std::map<std::string, VUI::UIElement*> elements;

	// This assumes "ui" is the first element on the stack, set up by ParseUI()
	lua_getfield(lua, 1, typeString.c_str());
	if (lua_isnil(lua, -1))
	{
		lua_pop(lua, 1);
		return std::map<std::string,VUI::UIElement*>();
	}
	else if (!lua_istable(lua, -1))
	{
		lua_pop(lua, 1);
		VUI::Log(VUI::ERROR_MINOR, std::string("Malformed table: \"" + typeString + "\" was not a table").c_str());
		return std::map<std::string,VUI::UIElement*>();
	}

	lua_pushnil(lua);
	while (lua_next(lua, 2) != 0)
	{
		if (!lua_isstring(lua, -2))
		{
			VUI::Log(VUI::ERROR_MINOR, std::string("UI category \"" + typeString + "\" contained a value, skipping").c_str());
			continue;
		}
		VUI::UIElement* element = VUI::ParseProperties(lua, -1, lua_tostring(lua, -2));
		elements.insert({element->id,element});
		lua_pop(lua, 1);
	}
	lua_settop(lua, 1);
	return elements;
}

bool VUI::HasValidInt(lua_State* lua, int elementIndex, std::string propertyName)
{
	lua_getfield(lua, elementIndex, propertyName.c_str());
	if (lua_isnumber(lua, -1))
	{
		lua_pop(lua, 1);
		return true;
	}
	lua_pop(lua, 1);
	return false;
}

bool VUI::HasValidVec2(lua_State* lua, int elementIndex, std::string propertyName)
{
	lua_getfield(lua, elementIndex, propertyName.c_str());
	if (lua_istable(lua, -1))
	{
		lua_getfield(lua, -1, "x");
		lua_getfield(lua, -2, "y");
		if (lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			lua_pop(lua, 3);
			return true;
		}
		lua_pop(lua, 3);
		return false;
	}
	lua_pop(lua, 1);
	return false;
}

bool VUI::HasValidString(lua_State* lua, int elementIndex, std::string propertyName)
{
	lua_getfield(lua, elementIndex, propertyName.c_str());

	if (!lua_isstring(lua, -1))
	{
		lua_pop(lua, 1);
		return false;
	}
	lua_pop(lua, 1);
	return true;
}

bool VUI::HasValidColor(lua_State* lua, int elementIndex, std::string propertyName)
{
	lua_getfield(lua, elementIndex, propertyName.c_str());

	if (!lua_istable(lua, -1))
	{
		lua_pop(lua, 1);
		return false;
	}

	lua_getfield(lua, -1, "r");
	lua_getfield(lua, -2, "g");
	lua_getfield(lua, -3, "b");
	lua_getfield(lua, -4, "a");

	if (lua_isnumber(lua, -1) && lua_isnumber(lua, -2) && lua_isnumber(lua, -3) && lua_isnumber(lua, -4))
	{
		bool valid = true;
		for (int i = 1; i < 4; i++)
		{
			valid &= lua_tointeger(lua, -i) <= 255 && lua_tointeger(lua, -i) >= 0;
		}
		lua_pop(lua, 5);
		return valid;
	}
	lua_pop(lua, 5);
	return false;
}

VUI::Math::u8vec4 inline VUI::GetColor(lua_State* lua, int elementIndex, std::string colorProperty)
{
	lua_getfield(lua, elementIndex, colorProperty.c_str());

	lua_getfield(lua, -1, "r");
	lua_getfield(lua, -2, "g");
	lua_getfield(lua, -3, "b");
	lua_getfield(lua, -4, "a");

	VUI::Math::u8vec4 color((uint8_t)lua_tointeger(lua, -4), (uint8_t)lua_tointeger(lua, -3), (uint8_t)lua_tointeger(lua, -2), (uint8_t)lua_tointeger(lua, -1));

	lua_pop(lua, 5);
	return color;
}

std::string VUI::GetString(lua_State* lua, int elementIndex, std::string stringProperty)
{
	lua_getfield(lua, elementIndex, stringProperty.c_str());
	std::string str = lua_tostring(lua, -1);
	lua_pop(lua, 1);
	return str;
}
VUI::Math::vec2 VUI::GetVec2(lua_State* lua, int elementIndex, std::string vecProperty)
{
	lua_getfield(lua, elementIndex, vecProperty.c_str());

	lua_getfield(lua, -1, "x");
	lua_getfield(lua, -2, "y");

	VUI::Math::vec2 vector(lua_tonumber(lua, -2), lua_tonumber(lua, -1));

	lua_pop(lua, 3);
	return vector;
}

int32_t VUI::GetInt(lua_State* lua, int elementIndex, std::string intProperty)
{
	lua_getfield(lua, -1, intProperty.c_str());
	int32_t val = lua_tointeger(lua, -1);
	lua_pop(lua, 1);
	return val;
}

VUI::UIElement* VUI::ParseProperties(lua_State* lua, int elementIndex, std::string elementName)
{
	UIElement* newElement = new UIElement();
	if (!lua_istable(lua, -1))
	{
		VUI::Log(VUI::ERROR_MINOR, std::string("ParseGenericProperties: Malformed element \"" + elementName + "\" was not a table, continuing with default properties").c_str());
		return newElement;
	}

	newElement->id = elementName;

	if (HasValidString(lua, -1, "texture"))
		newElement->texture = GetString(lua, -1, "texture");
	else
		newElement->texture = std::any_cast<std::string>(propertyDefaults.at("texture"));

	if (HasValidVec2(lua, -1, "position"))
		newElement->position = GetVec2(lua, -1, "position");
	else
		newElement->position = std::any_cast<Math::vec2>(propertyDefaults.at("position"));

	if (HasValidVec2(lua, -1, "dimensions"))
		newElement->dimensions = GetVec2(lua, -1, "dimensions");
	else
		newElement->dimensions = std::any_cast<Math::vec2>(propertyDefaults.at("dimensions"));

	if (HasValidColor(lua, -1, "color"))
		newElement->color = GetColor(lua, -1, "color");
	else
		newElement->color = std::any_cast<Math::u8vec4>(propertyDefaults.at("color"));

	if (HasValidInt(lua, -1, "layer"))
		newElement->layer = GetInt(lua, -1, "layer");
	else
		newElement->layer = std::any_cast<int32_t>(propertyDefaults.at("layer"));

	return newElement;
}