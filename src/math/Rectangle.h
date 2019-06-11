#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

namespace milk {
	struct Rectangle {
		int x;
		int y;
		int width;
		int height;

		Rectangle(int x = 0, int y = 0, int width = 0, int height = 0)
			: x(x)
			, y(y)
			, width(width)
			, height(height) {
		}

		int top();
		int bottom();
		int left();
		int right();
		bool overlaps(Rectangle other);
		bool empty();
	};

	inline int Rectangle::top() {
		return y;
	}

	inline int Rectangle::bottom() {
		return y + height;
	}

	inline int Rectangle::left() {
		return x;
	}

	inline int Rectangle::right() {
		return x + width;
	}

	inline bool Rectangle::overlaps(Rectangle other) {
		return other.left() < right()
			&& left() < other.right()
			&& other.top() < bottom()
			&& top() < other.bottom();
	}

	inline bool Rectangle::empty() {
		return x == 0
			&& y == 0
			&& width == 0
			&& height == 0;
	}
}

#endif
