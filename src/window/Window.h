#ifndef MILK_WINDOW_H
#define MILK_WINDOW_H

#include <string>

namespace milk
{
	class Window {
	public:
		virtual ~Window() = default;

		virtual bool fullscreen() const = 0;
		virtual void fullscreen(bool fs) = 0;
	};
}

#endif
