#pragma once

#include <vector>
#include <string>

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
				return { x + rhs.x, y + rhs.y};
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

			u8vec4 operator+(u8vec4 const& rhs)
			{
				u8vec4 newVec{ r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a };
				return newVec;
			}
			u8vec4(const int shade)
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
				r = 0;
				g = 0;
				b = 0;
				a = 0;
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

	static std::string currMessage;
	static bool isMessage;
	static uint8_t currErrorCode;
	struct Message
	{
		std::string message;
		uint8_t errorCode;
	};
	Message FetchMessage();

	void StartFrame();
	void Render();
	void EndFrame();

	void ParseUI(const char* filepath);
	void Log(ErrorCode, const char* message);

	struct UIElement
	{
		Math::vec2 position;
		Math::vec2 dimensions;
		Math::u8vec4 color;
		int32_t layer;
	};

	static std::vector<UIElement*> elements;
}