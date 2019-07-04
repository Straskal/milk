#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <tuple>

#include "data/int.h"

namespace milk
{
	struct Image;

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual std::tuple<int, int> resolution() const = 0;
		virtual void resolution(const int w, const int h) = 0;

		virtual void setDrawColor(const double r, const double g, const double b, const double a) = 0;
		virtual void drawRectangle(const float x, const float y, const float w, const float h) = 0;
		virtual void drawRectangleFilled(const float x, const float y, const float w, const float h) = 0;
		virtual void draw(const Image* image, const float x, const float y) = 0;
		virtual void draw(
			const Image* image,
			const float x, const float y,
			const int srcx, const int srcy, const int srcw, const int srch,
			const float scalex, const float scaley, const double angle) = 0;
	};
}

#endif
