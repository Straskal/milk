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
		void resolution(int w, int h) override;

		void setDrawColor(double r, double g, double b, double a) override;
		void drawRectangle(float x, float y, float w, float h) override;
		void drawRectangleFilled(float x, float y, float w, float h) override;
		void draw(const Image* image, float x, float y) override;
		void draw(const Image* image, float x, float y, int srcx, int srcy, int srcw, int srch, float scx, float scy, double angle) override;

	private:
		SDL_Renderer* m_handle;
		SDL_Color m_drawColor;
	};
}

#endif
