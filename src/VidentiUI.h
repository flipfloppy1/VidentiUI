#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <any>
#include "VidentiRenderer.h"
#include "VidentiMath.h"
#include "lua.hpp"
#include "VidentiPoller.h"

namespace VUI
{

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
		std::string font;
		Math::vec2 textPos;
		int32_t textSize;
		Math::u8vec4 fontColor;
		bool ratioTransform[2];
		int32_t layer;
		virtual VUI::Renderer::ElementVertices GenVerts();
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

		friend class UIElement;
	public:
		void Init();
		void StartFrame();
		void Render();
		void EndFrame();
		void ParseUI(const char* filepath);
		void GenUI();
		void RefreshEvents();
		void SetLuaGlobals(float deltaTime);
		void CollectLuaSignals();
		bool HasSignalled(std::string signal);
		bool HasSignalled(const char* signal);
		void AttachRenderer(Renderer::VidentiRenderer* renderer, Math::vec2 windowDimensions);
		void AttachPoller(Poller::VidentiPoller* poller);
		void DisableSignal(std::string signal);
		void DisableSignal(const char* signal);
		std::string GetLuaNextScript();

		std::string nextScript = "";
		std::map<std::string, UIElement*> elements;
		std::map<std::string, bool> signals;
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
		Poller::VidentiPoller* uiPoller = nullptr;
		void InitRenderer();
		void InitLua();
		void InitPoller();
		Math::vec2 windowDimensions;
		lua_State* lua = nullptr;
	};
	const std::unordered_map<std::string, std::any> propertyDefaults
	{
		{"dimensions",std::any(Math::vec2(0.1,0.1))},
		{"position",std::any(Math::vec2(0.0,0.0))},
		{"layer",std::any(int32_t(0))},
		{"color",std::any(Math::u8vec4(255,255,255,255))},
		{"fontColor",std::any(Math::u8vec4(255,255,255,255))},
		{"texture",std::any(std::string(""))},
		{"textPos",std::any(Math::vec2(0.0,0.0))},
		{"textSize",std::any(int32_t(0)) },
		{"font", std::any(std::string(""))},
		{"text", std::any(std::string(""))}
	};
}

