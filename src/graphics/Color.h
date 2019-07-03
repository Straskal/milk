#ifndef _COLOR_H_
#define _COLOR_H_

#include "data/int.h"

namespace milk
{
	struct Color
	{
		u8 r, b, g, a;

		Color(u8 r = 0x00, u8 blue = 0x00, u8 green = 0x00, u8 alpha = 0x00)
			: r(r)
			, b(blue)
			, g(green)
			, a(alpha)
		{
		}

		static Color clear();
		static Color black();
		static Color red();
	};

	inline Color Color::clear()
	{
		return Color{ 0x00, 0x00, 0x00, 0x00 };
	}

	inline Color Color::black()
	{
		return Color{ 0x00, 0x00, 0x00, 0xFF };
	}

	inline Color Color::red()
	{
		return Color{ 0xFF, 0x00, 0x00, 0xFF };
	}
}

#endif
