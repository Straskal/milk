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

			bool init(SDL_Window* windowHandle);
			void clear(const Color& color) override;
			void drawRectangle(const Rectangle& destinationRectangle, const Color& color) override;
			void draw(const Texture& texture, const Rectangle& sourceRectangle, const Rectangle& destinationRectangle, u8 flipFlags) override;
			void present() override;
			Resolution resolution() const override;
			void resolution(int w, int h) override;
			void free();

		private:
			Resolution m_resolution;
			SDL_Renderer* m_handle;
		};
	}
}

#endif
