
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
	uiRenderer->StartFrame();
}

void VUI::VidentiHandler::EndFrame()
{
	uiRenderer->EndFrame();
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
		std::string errorStr = lua_tostring(lua, -1);
		VUI::Log(ERROR_MAJOR, std::string("ParseUI: Lua was unable to load/run file \"" + std::string(filepath) + "\" with error: \"" + errorStr + "\" returning with empty UI").c_str());
		elements.clear();
		return;
	}


	lua_getglobal(lua, "ui");

	if (lua_isnil(lua, -1))
	{
		if (!elements.empty() && uiRenderer != nullptr)
			uiRenderer->CleanCompiledRender();
		elements.clear();
		return;
	}

	if (!lua_istable(lua, -1))
	{
		VUI::Log(ERROR_MAJOR, std::string("ParseUI: Variable \'ui\' was not a table in \"" + std::string(filepath) + "\", returning with empty UI").c_str());
		elements.clear();
		return;
	}

	std::map<std::string, UIElement*> newElements;

	newElements = ParseObjects(lua, "buttons");
	for (auto iter = newElements.begin(); iter != newElements.end(); iter++)
	{
		elements.insert_or_assign(iter->first, iter->second);
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
		return;

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

void VUI::VidentiHandler::AttachPoller(VUI::Poller::VidentiPoller* poller)
{
	uiPoller = poller;
}

void VUI::VidentiHandler::RefreshEvents()
{
	if (uiPoller == nullptr)
	{
		Log(VUI::ERROR_MAJOR, "VidentiHandler::RefreshEvents: UI Poller has not been defined, attach using AttachPoller()");
		return;
	}

	uiPoller->RefreshEvents();
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
VUI::Renderer::ElementVertices VUI::UIElement::GenVerts()
{
	std::vector<Renderer::UIVertex> textVertices;
	std::vector<Renderer::UIVertex> vertices;
	Math::vec2 windowDimensions = uiHandler->GetWindowDimensions();

	if (text != "")
	{
		float advance = 0.0f;
		for (int i = 0; i < text.size(); i++)
		{
			VUI::Renderer::VidentiRenderer::TexInfo texInfo = uiHandler->uiRenderer->GetTypeFaceInfo(font);
			VUI::Renderer::VidentiRenderer::GlyphInfo glyphInfo = uiHandler->uiRenderer->GetGlyphInfo(font, text[i]);
			Math::vec2 texPos = glyphInfo.pos / Math::vec2{ (float)texInfo.width, (float)texInfo.height };
			Math::vec2 texDim = glyphInfo.dim / Math::vec2{ (float)texInfo.width, (float)texInfo.height };
			glyphInfo.advance /= windowDimensions.x;
			glyphInfo.dim = (Math::vec2{ glyphInfo.dim.x - glyphInfo.pos.x, glyphInfo.pos.y - glyphInfo.dim.y }) / windowDimensions;
			glyphInfo.offset /= windowDimensions;
			glyphInfo.pos /= windowDimensions;
			Math::vec2 pos = { (textPos.x + advance + glyphInfo.offset.x) * 2.0f - 1.0f, -((textPos.y - glyphInfo.offset.y) * 2.0f - 1.0f)};
			Math::vec2 dim = { (glyphInfo.dim.x) * 2.0f, -(glyphInfo.dim.y * 2.0f) };
			textVertices.push_back(Renderer::UIVertex(Math::vec2{ pos.x, pos.y }, fontColor, { texPos.x, texDim.y }));
			textVertices.push_back(Renderer::UIVertex(Math::vec2{ pos.x, pos.y + dim.y }, fontColor, texPos));
			textVertices.push_back(Renderer::UIVertex(Math::vec2{ pos.x + dim.x, pos.y + dim.y }, fontColor, { texDim.x,texPos.y }));
			textVertices.push_back(Renderer::UIVertex(Math::vec2{ pos.x + dim.x, pos.y + dim.y }, fontColor, { texDim.x, texPos.y }));
			textVertices.push_back(Renderer::UIVertex(Math::vec2{ pos.x + dim.x, pos.y }, fontColor, texDim));
			textVertices.push_back(Renderer::UIVertex(Math::vec2{ pos.x, pos.y }, fontColor, { texPos.x,texDim.y }));

			advance += glyphInfo.advance;
		}
	}

	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f) }, color, { 0.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f + dimensions.y * 2.0f) }, color, { 0.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + dimensions.x * 2.0f, -(position.y * 2.0f - 1.0f + dimensions.y * 2.0f) }, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + dimensions.x * 2.0f, -(position.y * 2.0f - 1.0f + dimensions.y * 2.0f) }, color, { 1.0f,0.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f + dimensions.x * 2.0f, -(position.y * 2.0f - 1.0f) }, color, { 1.0f,1.0f }));
	vertices.push_back(Renderer::UIVertex(Math::vec2{ position.x * 2.0f - 1.0f, -(position.y * 2.0f - 1.0f) }, color, { 0.0f,1.0f }));
	return { vertices, textVertices };
}

void VUI::VidentiHandler::InitPoller()
{
	uiPoller->Init();
}

void VUI::VidentiHandler::InitRenderer()
{
	if (uiRenderer == nullptr)
		VUI::Log(ERROR_FATAL, "VidentiHandler::InitRenderer: Renderer was not attached before initialisation, call AttachRenderer(renderer, windowDimensions) before Init()");
	uiRenderer->Init();
	uiRenderer->SetWindowDimensions(windowDimensions);
	uiRenderer->InitFreetype();
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
	InitPoller();
}

std::string VUI::VidentiHandler::GetLuaNextScript()
{
	lua_getglobal(lua, "VUI_nextScript");
	if (lua_isnil(lua, -1))
		nextScript = "";
	if (lua_isstring(lua, -1))
		nextScript = std::string(lua_tostring(lua, -1));
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

	VUI::Poller::MouseState mouseState = uiPoller->GetMouseState();

	lua_createtable(lua, 3, 2);
	lua_setglobal(lua, "VUI_mouse");

	lua_getglobal(lua, "VUI_mouse");
	int luaMouseIndex = lua_gettop(lua);
	lua_pushboolean(lua, mouseState.mouseDownCurr && !mouseState.mouseDownPrev);
	lua_setfield(lua, luaMouseIndex, "onPress");
	lua_pushboolean(lua, mouseState.mouseDownPrev && !mouseState.mouseDownCurr);
	lua_setfield(lua, luaMouseIndex, "onRelease");
	lua_pushboolean(lua, mouseState.mouseDownCurr);
	lua_setfield(lua, luaMouseIndex, "down");
	lua_pushnumber(lua, mouseState.mouseX / windowDimensions.x);
	lua_setfield(lua, luaMouseIndex, "x");
	lua_pushnumber(lua, mouseState.mouseY / windowDimensions.y);
	lua_setfield(lua, luaMouseIndex, "y");
	lua_pop(lua, 1);
	lua_newtable(lua);
	lua_setglobal(lua, "VUI_signal");

	lua_newtable(lua);
	lua_setglobal(lua, "VUI_key");
	lua_getglobal(lua, "VUI_key");
	int luaKeyIndex = lua_gettop(lua);
	VUI::Poller::KeyMap keyMap = uiPoller->GetKeyStates();
	for (unsigned char c = 0; c < 255; c++)
	{
		VUI::Poller::KeyState state = {};
		if (keyMap.contains(c))
			state = keyMap.at(c);

		std::string keyStr = "";
		keyStr += c;
		lua_newtable(lua);
		lua_setfield(lua, luaKeyIndex, keyStr.c_str());
		lua_getfield(lua, luaKeyIndex, keyStr.c_str());
		lua_pushboolean(lua, state.pressCurr);
		lua_setfield(lua, -2, "down");
		lua_pushboolean(lua, state.pressCurr && !state.pressPrev);
		lua_setfield(lua, -2, "onPress");
		lua_pushboolean(lua, state.pressPrev && !state.pressCurr);
		lua_setfield(lua, -2, "onRelease");
		lua_pop(lua, 1);
	}

	lua_pop(lua, 1);
}

void VUI::VidentiHandler::CollectLuaSignals()
{
	lua_getglobal(lua, "VUI_signal");
	if (lua_isnil(lua, -1))
	{
		lua_pop(lua, 1);
		return;
	}

	int tableIndex = lua_gettop(lua);

	lua_pushnil(lua);
	while (lua_next(lua, tableIndex) != 0)
	{
		if (!lua_isstring(lua, -2) || !lua_isboolean(lua,-1))
		{
			lua_pop(lua, 1);
			continue;
		}

		signals[lua_tostring(lua, -2)] = lua_toboolean(lua, -1);
		lua_pop(lua, 1);
	}
}

bool VUI::VidentiHandler::HasSignalled(std::string signal)
{
	if (signals.contains(signal))
		return signals.at(signal);

	return false;
}

bool VUI::VidentiHandler::HasSignalled(const char* signal)
{
	if (signals.contains(signal))
		return signals.at(signal);

	return false;
}

void VUI::VidentiHandler::DisableSignal(std::string signal)
{
	if (signals.contains(signal))
		signals.at(signal) = false;
}

void VUI::VidentiHandler::DisableSignal(const char* signal)
{
	if (signals.contains(signal))
		signals.at(signal) = false;
}