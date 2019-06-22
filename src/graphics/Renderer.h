#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <tuple>

#include "data/int.h"
#include "math/Vector2.h"

namespace milk {
	struct Texture;
	struct Color;
	struct Rectangle;

	enum FlipFlags {
		NO_FLIP,
		FLIP_X = 1 << 0,
		FLIP_Y = 1 << 1
	};

	class Renderer {
	public:
		virtual ~Renderer() = default;

		virtual void clear() = 0;
		virtual void setDrawColor(const Color* color) = 0;
		virtual void drawRectangle(const Rectangle* destinationRectangle) = 0;
		virtual void drawRectangleFilled(const Rectangle* destinationRectangle) = 0;
		virtual void draw(const Texture* texture, const Rectangle* sourceRectangle, const Rectangle* destinationRectangle, u8 flipFlags) = 0;
		virtual void present() = 0;
		virtual std::tuple<int, int> resolution() const = 0;
		virtual void resolution(int w, int h) = 0;
	};
}

#endif