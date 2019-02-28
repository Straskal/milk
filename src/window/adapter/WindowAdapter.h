#ifndef MILK_WINDOW_ADAPTER_H
#define MILK_WINDOW_ADAPTER_H

// We define this bad boi here because SDL does not, and we don't want magic numbers chilling up in this boi.
#define MILK_SDL_WINDOW_NO_FLAG 0

#include <string>

#include "window/Window.h"

struct SDL_Window;

namespace milk
{
	namespace adapter
	{
		/// SDL implementation of Window.
		class WindowAdapter : public Window
		{
		public:
			static WindowAdapter& instance()
			{
				static WindowAdapter instance;
				return instance;
			}

			bool init(const std::string& title, unsigned int width, unsigned int height, bool fullscreen);

			unsigned int width() const override;

			unsigned int height() const override;

			bool fullscreen() const override;

			void toggleFullscreen() override;

			SDL_Window* sdlWindow() const;

			void free();

		private:
			WindowAdapter();

			std::string title_;

			unsigned int width_;
			unsigned int height_;

			bool fullscreen_;

			bool initialized_;

			SDL_Window* sdlWindow_;
		};
	}
}

#endif
