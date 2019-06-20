#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <tuple>

namespace milk {
	enum MouseButtons {
		MOUSE_LEFT = 1,
		MOUSE_MIDDLE,
		MOUSE_RIGHT
	};

	class Mouse {
	public:
		virtual ~Mouse() = default;

		virtual std::tuple<int, int> getPosition() const = 0;
		virtual bool isButtonDown(MouseButtons button) const = 0;
		virtual bool isButtonPressed(MouseButtons button) const = 0;
		virtual bool isButtonReleased(MouseButtons button) const = 0;
	};
}

#endif