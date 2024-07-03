#pragma once

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
		virtual std::vector<Renderer::UIVertex> GenVerts() = 0;
	protected:
		VidentiHandler* uiHandler;
	};

	class Button : public UIElement
	{
		virtual std::vector<Renderer::UIVertex> GenVerts();
	};

	class Slider : public UIElement
	{
		virtual std::vector<Renderer::UIVertex> GenVerts();
	};

	class Texture : public UIElement
	{
		virtual std::vector<Renderer::UIVertex> GenVerts();
	};

	class Rectangle : public UIElement
	{
		virtual std::vector<Renderer::UIVertex> GenVerts();
	};

	class Text : public UIElement
	{
		virtual std::vector<Renderer::UIVertex> GenVerts();
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
		void AttachRenderer(Renderer::VidentiRenderer* renderer, Math::vec2 windowDimensions);

		std::vector<UIElement*> elements;

		inline UIElement* FindElement(std::string id)
		{
			for (UIElement* element : elements)
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
		void SetWindowDimensions()
		{

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
		{"ratioTransform",std::any(false)}
	};
}