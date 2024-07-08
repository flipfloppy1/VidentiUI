﻿#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <any>
#include "VidentiRenderer.h"
#include "VidentiMath.h"
#include "lua.hpp"

namespace VUI
{
	namespace Renderer
	{
		class VidentiRenderer;
	}

	enum ErrorCode : uint8_t
	{
		ERROR_NONE,
		ERROR_WARNING,
		ERROR_MINOR,
		ERROR_MAJOR,
		ERROR_FATAL
	};

	class VidentiHandler;
	class VidentiRenderer;
	class UIElement
	{
		friend class VidentiHandler;

	public:
		std::string id;
		Math::vec2 position;
		Math::vec2 dimensions;
		Math::u8vec4 color;
		std::string text;
		std::string texture;
		bool ratioTransform[2];
		int32_t layer;
		virtual std::vector<Renderer::UIVertex> GenVerts();
	protected:
		VidentiHandler* uiHandler;
	};

	struct Message
	{
		std::string message;
		uint8_t errorCode;
	};

	static std::string currMessage;
	static bool isMessage;
	static uint8_t currErrorCode;

	Message FetchMessage();
	void Log(ErrorCode, const char* message);

	class VidentiHandler
	{
	public:
		void Init();
		void StartFrame();
		void Render();
		void EndFrame();
		void ParseUI(const char* filepath);
		void GenUI();
		void SetLuaGlobals(float deltaTime);
		void AttachRenderer(Renderer::VidentiRenderer* renderer, Math::vec2 windowDimensions);
		std::string* GetLuaNextScript();

		std::string* nextScript = nullptr;
		std::map<std::string, UIElement*> elements;

		inline UIElement* FindElement(std::string id)
		{
			for (auto [id, element] : elements)
			{
				if (element->id == id)
					return element;
			}
			return nullptr;
		}

		Math::vec2 GetWindowDimensions()
		{
			if (uiRenderer == nullptr)
			{
				Log(VUI::ERROR_MAJOR, "VidentiHandler::GetWindowDimensions: No renderer attached");
				return Math::vec2(0);
			}
			return uiRenderer->windowDimensions;
		}
		void SetWindowDimensions(Math::vec2 windowDimensions)
		{
			if (uiRenderer == nullptr)
			{
				Log(VUI::ERROR_MAJOR, "VidentiHandler::SetWindowDimensions: No renderer attached");
				return;
			}
			VidentiHandler::windowDimensions = windowDimensions;
			uiRenderer->windowDimensions = windowDimensions;
		}
	private:
		Renderer::VidentiRenderer* uiRenderer = nullptr;
		void InitRenderer();
		void InitLua();
		Math::vec2 windowDimensions;
		lua_State* lua = nullptr;
	};
	const std::unordered_map<std::string, std::any> propertyDefaults
	{
		{"dimensions",std::any(Math::vec2(0.1,0.1))},
		{"position",std::any(Math::vec2(0.0,0.0))},
		{"layer",std::any(int32_t(0))},
		{"color",std::any(Math::u8vec4(0,0,0,255))},
		{"texture",std::any(std::string(""))},
		{"ratioTransform",std::any(false)}
	};
}

