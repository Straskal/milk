#ifndef MILK_WINDOW_H
#define MILK_WINDOW_H

#include <string>

#include "math/Vector2.h"

namespace milk
{
	class Window {
	public:
		virtual ~Window() = default;

		virtual std::string title() const = 0;
		virtual void title(const std::string& title) = 0;
		virtual Vector2 size() const = 0;
		virtual void size(int width, int height) = 0;
		virtual bool fullscreen() const = 0;
		virtual void fullscreen(bool toggle) = 0;
		virtual void close() = 0;

	};
}

#endif
