#ifndef MILK_WINDOW_SDL_H
#define MILK_WINDOW_SDL_H

// We define this bad boi here because SDL does not, and we don't want magic numbers chilling up in this boi.
#define MILK_SDL_WINDOW_NO_FLAG 0

#include <string>

#include "window/Window.h"

struct SDL_Window;

namespace milk
{
	namespace sdl
	{
		class Window : public milk::Window{
		public:
			Window();

			bool init(); 
			std::string title() const override;
			void title(const std::string& title) override;
			Vector2 size() const override;
			void size(int width, int height) override;
			bool fullscreen() const override;
			void fullscreen(bool toggle) override;
			void show();
			SDL_Window* handle() const;
			void free();

		private:
			SDL_Window* m_handle;
			int m_width;
			int m_height;
		};
	}
}

#endif
