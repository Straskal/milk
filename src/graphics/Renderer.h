#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <tuple>

namespace milk
{
	struct Image;

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual std::tuple<int, int> resolution() const = 0;
		virtual void resolution(int w, int h) = 0;

		virtual void setDrawColor(double r, double g, double b, double a) = 0;
		virtual void drawRectangle(float x, float y, float w, float h) = 0;
		virtual void drawRectangleFilled(float x, float y, float w, float h) = 0;
		virtual void draw(const Image* image, float x, float y) = 0;
		virtual void draw(const Image* image, float x, float y, int srcx, int srcy, int srcw, int srch, float scx, float scy, double angle) = 0;
	};
}

#endif
