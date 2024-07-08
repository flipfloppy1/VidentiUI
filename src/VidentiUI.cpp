
#include "VidentiUI.h"
#include "VidentiParse.h"
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
		VUI::Log(ERROR_MAJOR, std::string("ParseUI: Unable to find file \"" + std::string(filepath) + "\", returning with empty UI").c_str());
		elements.clear();
		return;
	}
	f.close();

	// Clear stack
	lua_settop(lua, 0);

	if (luaL_dofile(lua, filepath))
	{
		VUI::Log(ERROR_MAJOR, std::string("ParseUI: Lua was unable to load/run file \"" + std::string(filepath) + "\", returning with empty UI").c_str());
		elements.clear();
		return;
	}

	
	lua_getglobal(lua, "ui");

	if (lua_isnil(lua, -1))
	{
		VUI::Log(ERROR_MINOR, std::string("ParseUI: Variable \'ui\' did not exist in \"" + std::string(filepath) + "\", returning with empty UI").c_str());
		elements.clear();
		return;
	}

	if (!lua_istable(lua, -1))
	{
		VUI::Log(ERROR_MAJOR, std::string("ParseUI: Variable \'ui\' was not a table in \"" + std::string(filepath) + "\", returning with empty UI").c_str());
		elements.clear();
		return;
	}

	std::map<std::string,UIElement*> newElements;
	
	newElements = ParseObjects(lua, "buttons");
	for (auto iter = newElements.begin(); iter != newElements.end(); iter++)
	{
		elements.insert_or_assign(iter->first,iter->second);
	}

	for (auto [id, element] : elements)
	{
		element->uiHandler = this;
	}

	lua_settop(lua, 0);
}

void VUI::VidentiHandler::GenUI()
{
	if (elements.empty())
	{
		Log(VUI::ERROR_MINOR, "VidentiHandler::GenUI: No elements to generate, call ParseUI() to add elements");
		return;
	}

	if (uiRenderer == nullptr)
	{
		Log(VUI::ERROR_MAJOR, "VidentiHandler::GenUI: UI Renderer has not been defined, attach using AttachRenderer()");
		return;
	}

	uiRenderer->InstructGeneration();
}

void VUI::VidentiHandler::AttachRenderer(VUI::Renderer::VidentiRenderer* renderer, Math::vec2 windowDimensions)
{
	VidentiHandler::windowDimensions = windowDimensions;
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
	std::cout << "Videnti log with code: " << errorString << "; message:\n \"" << message << "\"\n";
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

// Simple vertices for a box, for now
std::vector<VUI::Renderer::UIVertex> VUI::UIElement::GenVerts()
{
	std::vector<Renderer::UIVertex> vertices;
	Math::vec2 windowDimensions = uiHandler->GetWindowDimensions();

	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f) }, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f + dimensions.y * 2.0f) }, color, { 0.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + dimensions.x * 2.0f, -(position.y * 2.0f - 1.0f + dimensions.y * 2.0f) }, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + dimensions.x * 2.0f, -(position.y * 2.0f - 1.0f + dimensions.y * 2.0f) }, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + dimensions.x * 2.0f, -(position.y * 2.0f - 1.0f) }, color, { 1.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f) }, color, { 0.0f,1.0f }));
	return vertices;
}

void VUI::VidentiHandler::InitRenderer()
{
	if (uiRenderer == nullptr)
		VUI::Log(ERROR_FATAL, "VidentiHandler::InitRenderer: Renderer was not attached before initialisation, call AttachRenderer(renderer, windowDimensions) before Init()");
	uiRenderer->Init();
	uiRenderer->SetWindowDimensions(windowDimensions);
}
void VUI::VidentiHandler::InitLua()
{
	lua = luaL_newstate();
	luaL_openlibs(lua);
	luaopen_jit(lua);

	SetLuaGlobals(0.0f);
}

void VUI::VidentiHandler::Init()
{
	InitLua();
	InitRenderer();
}

std::string* VUI::VidentiHandler::GetLuaNextScript()
{
	lua_getglobal(lua, "VUI_nextScript");
	if (lua_isnil(lua, -1))
		nextScript = nullptr;
	if (lua_isstring(lua, -1))
		nextScript = new std::string(lua_tostring(lua, -1));
	lua_pop(lua, 1);
	return nextScript;
}

void VUI::VidentiHandler::SetLuaGlobals(float deltaTime)
{
	lua_pushnumber(lua, windowDimensions.x);
	lua_setglobal(lua, "VUI_winX");
	lua_pushnumber(lua, windowDimensions.y);
	lua_setglobal(lua, "VUI_winY");
	lua_pushnumber(lua, deltaTime);
	lua_setglobal(lua, "VUI_dt");
}