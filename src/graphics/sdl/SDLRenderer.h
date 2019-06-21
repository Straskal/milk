#ifndef _SDL_RENDERER_H_
#define _SDL_RENDERER_H_

#include "graphics/Renderer.h"

struct SDL_Renderer;
struct SDL_Window;

namespace milk {
	class SDLRenderer : public Renderer {
	public:
		SDLRenderer();

		bool init(SDL_Window* windowHandle);
		void clear() override;
		void setDrawColor(const Color* color) override;
		void drawRectangle(const Rectangle* destinationRectangle) override;
		void drawRectangleFilled(const Rectangle* destinationRectangle) override;
		void draw(const Texture* texture, const Rectangle* sourceRectangle, const Rectangle* destinationRectangle, u8 flipFlags) override;
		void present() override;
		std::tuple<int, int> resolution() const override;
		void resolution(int w, int h) override;
		SDL_Renderer* handle() const;
		void free();

	private:
		SDL_Renderer* m_handle;
	};
}

#endif
