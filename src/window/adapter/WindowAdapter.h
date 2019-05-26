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
		class WindowAdapter : public Window {
		public:
			WindowAdapter();

			bool init(const std::string& title, unsigned int width, unsigned int height, bool fullscreen) override;
			unsigned int width() const override;
			unsigned int height() const override;
			bool fullscreen() const override;
			void toggleFullscreen() override;
			void* handle() const override;
			void free() override;

		private:
			SDL_Window* handle_;
			int width_;
			int height_;
		};
	}
}

#endif
