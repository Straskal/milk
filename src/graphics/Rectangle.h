#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

namespace milk
{
	struct Rectangle
	{
		int x, y, w, h;

		Rectangle(int x = 0, int y = 0, int w = 0, int h = 0)
			: x(x)
			, y(y)
			, w(w)
			, h(h)
		{
		}
	};

	struct RectangleF
	{
		float x, y, w, h;

		RectangleF(float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f)
			: x(x)
			, y(y)
			, w(w)
			, h(h)
		{
		}
	};
}

#endif
