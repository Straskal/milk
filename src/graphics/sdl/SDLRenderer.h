#ifndef _SDL_RENDERER_H_
#define _SDL_RENDERER_H_

#include "SDL_render.h"
#include "SDL_rect.h"

#include "graphics/Renderer.h"

struct SDL_Renderer;
struct SDL_Window;

namespace milk
{
	class SDLRenderer : public Renderer
	{
	public:
		SDLRenderer();

		bool init(SDL_Window* windowHandle);
		void clear() override;
		void setDrawColor(const Color* color) override;
		void drawRectangle(const RectangleF* destinationRectangle) override;
		void drawRectangleFilled(const RectangleF* destinationRectangle) override;
		void draw(const Image* image, const Rectangle* sourceRectangle, const RectangleF* destinationRectangle, u8 flipFlags) override;
		void present() override;
		std::tuple<int, int> resolution() const override;
		void resolution(int w, int h) override;
		SDL_Renderer* handle() const;
		void free();

	private:
		SDL_Renderer* m_handle;

		// These values are cached so we don't create them with every draw call
		SDL_Color m_drawColor;
		SDL_Rect m_sourceRect;
		SDL_FRect m_destRect;
	};
}

#endif
