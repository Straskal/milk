#ifndef MILK_COLOR_H
#define MILK_COLOR_H

#include "data/int.h"

namespace milk {
	struct Color {
		U8 red;
		U8 blue;
		U8 green;
		U8 alpha;

		explicit Color(U8 red = 0x00, U8 blue = 0x00, U8 green = 0x00, U8 alpha = 0x00)
			: red(red)
			, blue(blue)
			, green(green)
			, alpha(alpha) {
		}

		static Color clear();
		static Color black();
	};

	inline Color Color::clear() {
		return Color{ 0x00, 0x00, 0x00, 0x00 };
	}

	inline Color Color::black() {
		return Color{ 0x00, 0x00, 0x00, 0xFF };
	}
}

#endif
