#ifndef MILK_RECTANGLE_H
#define MILK_RECTANGLE_H

namespace milk {
	struct Rectangle {
		Rectangle(int x = 0, int y = 0, int width = 0, int height = 0)
			: x(x), y(y), width(width), height(height) {
		}

		int x, y, width, height;

		int top() {
			return y;
		}

		int bottom() {
			return y + height;
		}

		int left() {
			return x;
		}

		int right() {
			return x + width;
		}

		// Returns true if rectangle overlaps other rectangle.
		bool overlaps(Rectangle other) {
			return other.left() < right()
				&& left() < other.right()
				&& other.top() < bottom()
				&& top() < other.bottom();
		}

		// Returns true if rectangle is empty.
		bool empty() {
			return x == 0 && y == 0 && width == 0 && height == 0;
		}
	};
}

#endif
