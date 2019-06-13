#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <tuple>

#include "math/Vector2.h"

namespace milk
{
	class Window {
	public:
		virtual ~Window() = default;

		virtual std::string title() const = 0;
		virtual void title(const std::string& title) = 0;
		virtual std::tuple<int, int> dimensions() const = 0;
		virtual void size(int width, int height) = 0;
		virtual bool fullscreen() const = 0;
		virtual void fullscreen(bool toggle) = 0;
		virtual void close() = 0;

	};
}

#endif
