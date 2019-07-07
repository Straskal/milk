#ifndef _SDL_WINDOW_H_
#define _SDL_WINDOW_H_

#include <string>

#include "window/Window.h"

struct SDL_Window;

namespace milk
{
	class SDLWindow : public Window
	{
	public:
		SDLWindow();

		bool init();
		SDL_Window* handle() const;
		void free();

		void show() override;
		const char* title() const override;
		void title(const char* title) override;
		void icon(const char* filepath) override;
		std::tuple<int, int> size() const override;
		void size(int width, int height) override;
		bool fullscreen() const override;
		void fullscreen(bool toggle) override;
		void minimize() override;
		void restore() override;
		void close() override;
		bool shouldClose() const override;

	private:
		SDL_Window* m_handle;
		bool m_shouldClose;
	};
}

#endif
