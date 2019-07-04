#ifndef _SDL_RENDERER_H_
#define _SDL_RENDERER_H_

#include "SDL.h"

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
		void free();
		SDL_Renderer* handle() const;

		void clear();
		void present();

		std::tuple<int, int> resolution() const override;
		void resolution(const int w, const int h) override;

		void setDrawColor(const double r, const double g, const double b, const double a) override;
		void drawRectangle(const float x, const float y, const float w, const float h) override;
		void drawRectangleFilled(const float x, const float y, const float w, const float h) override;
		void draw(const Image* image, const float x, const float y) override;
		void draw(
			const Image* image,
			const float x, const float y,
			const int srcx, const int srcy, const int srcw, const int srch,
			const float scalex, const float scaley, const double angle) override;

	private:
		SDL_Renderer* m_handle;

		// These values are cached so we don't create them with every draw call
		SDL_Color m_drawColor;
		SDL_Rect m_sourceRect;
		SDL_FRect m_destRect;
	};
}

#endif
