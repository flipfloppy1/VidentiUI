#pragma once

#include "lua.hpp"
#include "VidentiUI.h"
#include "VidentiMath.h"
#include <iostream>
#include <map>

namespace VUI
{
	bool HasValidVec2(lua_State* lua, int elementIndex, std::string propertyName);
	bool HasValidString(lua_State* lua, int elementIndex, std::string propertyName);
	bool HasValidColor(lua_State* lua, int elementIndex, std::string propertyName);
	bool HasValidInt(lua_State* lua, int elementIndex, std::string propertyName);

	Math::u8vec4 inline GetColor(lua_State* lua, int elementIndex, std::string colorProperty);
	Math::vec2 inline GetVec2(lua_State* lua, int elementIndex, std::string vecProperty);
	std::string GetString(lua_State* lua, int elementIndex, std::string stringProperty);
	int32_t GetInt(lua_State* lua, int elementIndex, std::string intProperty);

	std::map<std::string,UIElement*> ParseObjects(lua_State* lua, std::string typeString);
	UIElement* ParseProperties(lua_State* lua, int indexElement, std::string elementName);
}