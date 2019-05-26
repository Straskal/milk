#ifndef MILK_RENDERER_ADAPTER_H
#define MILK_RENDERER_ADAPTER_H

#include "window/Renderer.h"

struct SDL_Renderer;
struct SDL_Window;

namespace milk {
	namespace adapter {
		class RendererAdapter : public Renderer {
		public:
			RendererAdapter();

			bool init(void* windowHandle, unsigned int resolutionWidth, unsigned int resolutionHeight) override;
			void clear(const Color& color) override;
			void drawRectangle(const Rectangle& destinationRectangle, const Color& color) override;
			void drawRectangleOutline(const Rectangle& destinationRectangle, const Color& color) override;
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
