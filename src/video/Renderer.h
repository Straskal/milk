#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <tuple>

#include "data/int.h"
#include "math/Vector2.h"

namespace milk
{
	struct Texture;
	struct Color;
	struct Rectangle;

	class Renderer {
	public:
		virtual ~Renderer() = default;

		virtual void clear() = 0;
		virtual void drawRectangle(const Rectangle* destinationRectangle, const Color* color) = 0;
		virtual void draw(const Texture* texture, const Rectangle* sourceRectangle, const Rectangle* destinationRectangle, u8 flipFlags) = 0;
		virtual void present() = 0;
		virtual std::tuple<int, int> resolution() const = 0;
		virtual void resolution(int w, int h) = 0;
	};
}

#endif
