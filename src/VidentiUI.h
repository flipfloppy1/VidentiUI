#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <any>

namespace VUI
{
	namespace Math
	{
		struct vec2
		{
			double x;
			double y;
			inline vec2 operator+(vec2 const& rhs)
			{
				return { x + rhs.x, y + rhs.y };
			}
			inline vec2 operator-(vec2 const& rhs)
			{
				return { x - rhs.x, y - rhs.y };
			}
			inline vec2 operator/(float const& rhs)
			{
				if (rhs == 0.0)
					return vec2(0.0);
				return { x / rhs,y / rhs };
			}
			inline vec2 operator/(double const& rhs)
			{
				if (rhs == 0.0)
					return vec2(0.0);
				return { x / rhs,y / rhs };
			}
			inline vec2 operator*(float const& rhs)
			{
				return { x * rhs,y * rhs };
			}
			inline vec2 operator*(double const& rhs)
			{
				return { x * rhs,y * rhs };
			}

			vec2(double x, double y)
			{
				vec2::x = x;
				vec2::y = y;
			}
			vec2(double constant)
			{
				x = constant;
				y = constant;
			}
			vec2()
			{
				vec2::x = 0.0;
				vec2::y = 0.0;
			}
		};
		struct alignas(1) u8vec4
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;

			u8vec4 operator*(u8vec4 const& rhs)
			{
				u8vec4 newVec{ uint8_t((float)r * (rhs.r / 255.0f)), uint8_t((float)g + (rhs.g / 255.0f)), uint8_t((float)b + (rhs.b / 255.0f)), uint8_t((float)a + (rhs.a / 255.0f)) };
				return newVec;
			}
			bool operator==(u8vec4 const& rhs)
			{
				return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
			}
			u8vec4(const uint8_t shade)
			{
				r = shade;
				g = shade;
				b = shade;
				a = shade;
			}
			u8vec4(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{
				u8vec4::r = r;
				u8vec4::g = g;
				u8vec4::b = b;
				u8vec4::a = a;
			}
			u8vec4()
			{
				r = (uint8_t)0;
				g = (uint8_t)0;
				b = (uint8_t)0;
				a = (uint8_t)0;
			}
		};
	}

	enum ErrorCode : uint8_t
	{
		ERROR_NONE,
		ERROR_WARNING,
		ERROR_MINOR,
		ERROR_MAJOR,
		ERROR_FATAL
	};

	struct UIElement
	{
		std::string id;
		Math::vec2 position;
		Math::vec2 dimensions;
		Math::u8vec4 color;
		int32_t layer;
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

	class UIHandler
	{
	public:

		void StartFrame();
		void Render();
		void EndFrame();

		void ParseUI(const char* filepath);

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
	};
	const std::unordered_map<std::string, std::any> propertyDefaults
	{
		{"dimensions",std::any(Math::vec2(0.1,0.1))},
		{"position",std::any(Math::vec2(0.0,0.0))},
		{"layer",std::any(int32_t(0))},
		{"color",std::any(Math::u8vec4(255,255,255,255))}
	};
}