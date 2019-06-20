#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <tuple>

namespace milk {
	class Mouse {
	public:
		virtual ~Mouse() = default;

		virtual std::tuple<int, int> getPosition() const = 0;
		virtual bool isLeftButtonPressed() const = 0;
		virtual bool isRightButtonPressed() const = 0;
		virtual bool wasLeftButtonPressed() const = 0;
		virtual bool wasRightButtonPressed() const = 0;
		virtual bool wasLeftButtonReleased() const = 0;
		virtual bool wasRightButtonReleased() const = 0;
	};
}

#endif