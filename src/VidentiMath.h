#pragma once

#include <cinttypes>


namespace VUI
{
	namespace Math
	{
		struct vec2
		{
			float x;
			float y;
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
			inline vec2 operator*(float const& rhs)
			{
				return { x * rhs,y * rhs };
			}
			inline bool operator==(vec2 const& rhs)
			{
				return x == rhs.x && y == rhs.y;
			}
			inline vec2 operator/(vec2 const& rhs)
			{
				return { x / rhs.x, y / rhs.y };
			}
			inline void operator/=(vec2 const& rhs)
			{
				*this = *this / rhs;
			}
			// Creates a vec2 of { x, y }
			vec2(float x, float y)
			{
				vec2::x = x;
				vec2::y = y;
			}
			// Creates a vec2 of { constant, constant }
			vec2(float constant)
			{
				x = constant;
				y = constant;
			}
			// Creates a vec2 of { 0.0, 0.0 }
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
			inline bool operator==(u8vec4& rhs)
			{
				return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a);
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
}