#ifndef MILK_WINDOW_H
#define MILK_WINDOW_H

#include <string>

namespace milk
{
	class Window {
	public:
		virtual bool init(const std::string& title, unsigned int width, unsigned int height, bool fullscreen) = 0;
		virtual unsigned int width() const = 0;
		virtual unsigned int height() const = 0;
		virtual bool fullscreen() const = 0;
		virtual void toggleFullscreen() = 0;
		virtual void* handle() const = 0;
		virtual void free() = 0;
	};
}

#endif
