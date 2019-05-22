#ifndef MILK_RENDERER_H
#define MILK_RENDERER_H

#include "data/int.h"

namespace milk
{
	class Texture;
	struct Color;
	struct Rectangle;

	struct Resolution {
		unsigned int width;
		unsigned int height;
	};

	class Renderer {
	public:
		virtual void clear(const Color& color) = 0;
		virtual void drawRectangle(const Rectangle& destinationRectangle, const Color& color) = 0;
		virtual void drawRectangleOutline(const Rectangle& destinationRectangle, const Color& color) = 0;
		virtual void draw(const Texture& texture, const Rectangle& sourceRectangle, const Rectangle& destinationRectangle, U8 flipFlags) = 0;
		virtual void present() = 0;
		virtual Resolution resolution() const = 0;
	};
}

#endif
