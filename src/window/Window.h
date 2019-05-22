#ifndef MILK_WINDOW_H
#define MILK_WINDOW_H

namespace milk
{
	class Window {
	public:
		virtual unsigned int width() const = 0;
		virtual unsigned int height() const = 0;
		virtual bool fullscreen() const = 0;
		virtual void toggleFullscreen() = 0;
	};
}

#endif
