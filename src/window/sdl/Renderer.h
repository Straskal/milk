#ifndef MILK_RENDERER_SDL_H
#define MILK_RENDERER_SDL_H

#include "window/Renderer.h"

struct SDL_Renderer;
struct SDL_Window;

namespace milk {
	namespace sdl {
		class Renderer : public milk::Renderer {
		public:
			Renderer();

			bool init(void* windowHandle, int resolutionWidth, int resolutionHeight) override;
			void clear(const Color& color) override;
			void drawRectangle(const Rectangle& destinationRectangle, const Color& color) override;
			void draw(const Texture& texture, const Rectangle& sourceRectangle, const Rectangle& destinationRectangle, U8 flipFlags) override;
			void present() override;
			Resolution resolution() const override;
			void free() override;

		private:

			bool initialized_;
			Resolution resolution_;
			SDL_Renderer* handle_;
		};
	}
}

#endif
