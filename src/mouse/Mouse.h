#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <tuple>

namespace milk
{
	enum class MouseButtons
	{
		LEFT = 1,
		MIDDLE,
		RIGHT
	};

	class Mouse
	{
	public:
		virtual ~Mouse() = default;

		virtual std::tuple<int, int> getPosition() const = 0;
		virtual bool isButtonDown(const MouseButtons button) const = 0;
		virtual bool isButtonPressed(const MouseButtons button) const = 0;
		virtual bool isButtonReleased(const MouseButtons button) const = 0;
		virtual int getScroll() const = 0;
	};
}

#endif