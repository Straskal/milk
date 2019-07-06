#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <tuple>

namespace milk
{
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual const char* title() const = 0;
		virtual void title(const char* title) = 0;
		virtual void icon(const char* filepath) = 0;
		virtual std::tuple<int, int> size() const = 0;
		virtual void size(int width, int height) = 0;
		virtual bool fullscreen() const = 0;
		virtual void fullscreen(bool toggle) = 0;
		virtual void minimize() = 0;
		virtual void restore() = 0;
		virtual void close() = 0;
	};
}

#endif
