#include "SDLRenderer.h"

#include <algorithm>
#include <iostream>

#include "SDL.h"

#include "graphics/Image.h"

static const int FIRST_SUPPORTED_RENDERING_DRIVER = -1;
static const char* BATCHING_ON = "1";
static const char* NEAREST_PIXEL_SAMPLING = "nearest";

milk::SDLRenderer::SDLRenderer()
	: m_handle{ nullptr }
	, m_drawColor{ 0xFF, 0xFF, 0xFF, 0xFF } {}

bool milk::SDLRenderer::init(SDL_Window* windowHandle)
{
	m_handle = SDL_CreateRenderer(windowHandle, FIRST_SUPPORTED_RENDERING_DRIVER, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_handle == nullptr) {
		std::cout << "Error creating SDL_Renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	// Default the resolution to the window dimensions.
	int w, h;
	SDL_GetWindowSize(windowHandle, &w, &h);
	SDL_RenderSetLogicalSize(m_handle, w, h);

	SDL_SetHint(SDL_HINT_RENDER_BATCHING, BATCHING_ON);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, NEAREST_PIXEL_SAMPLING);
	SDL_SetRenderDrawBlendMode(m_handle, SDL_BLENDMODE_BLEND);
	return true;
}

void milk::SDLRenderer::free()
{
	SDL_DestroyRenderer(m_handle);
}

SDL_Renderer* milk::SDLRenderer::handle() const
{
	return m_handle;
}

std::tuple<int, int> milk::SDLRenderer::resolution() const
{
	int w, h;
	SDL_RenderGetLogicalSize(m_handle, &w, &h);
	return std::make_tuple(w, h);
}

void milk::SDLRenderer::resolution(int w, int h)
{
	SDL_RenderSetLogicalSize(m_handle, w, h);
}

void milk::SDLRenderer::clear()
{
	SDL_RenderClear(m_handle);
}

void milk::SDLRenderer::setDrawColor(double r, double g, double b, double a)
{
	m_drawColor.r = (Uint8)(std::min((int)(r * 0xFF), 0xFF));
	m_drawColor.g = (Uint8)(std::min((int)(g * 0xFF), 0xFF));
	m_drawColor.b = (Uint8)(std::min((int)(b * 0xFF), 0xFF));
	m_drawColor.a = (Uint8)(std::min((int)(a * 0xFF), 0xFF));
	SDL_SetRenderDrawColor(m_handle, m_drawColor.r, m_drawColor.g, m_drawColor.b, m_drawColor.a);
}

void milk::SDLRenderer::drawRectangle(float x, float y, float w, float h)
{
	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_RenderDrawRectF(m_handle, &dest);
}

void milk::SDLRenderer::drawRectangleFilled(float x, float y, float w, float h)
{
	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_RenderFillRectF(m_handle, &dest);
}

void milk::SDLRenderer::draw(const Image* image, float x, float y)
{
	SDL_Texture* texture = (SDL_Texture*)image->data->handle;
	SDL_SetTextureColorMod(texture, m_drawColor.r, m_drawColor.g, m_drawColor.b);
	SDL_SetTextureAlphaMod(texture, m_drawColor.a);

	int w = image->data->width;
	int h = image->data->height;

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = w;
	src.h = h;

	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = (float)w;
	dest.h = (float)h;

	SDL_RenderCopyExF(m_handle, texture, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
}

void milk::SDLRenderer::draw(const Image* image, float x, float y, int srcx, int srcy, int srcw, int srch, float scx, float scy, double angle)
{
	SDL_Texture* texture = (SDL_Texture*)image->data->handle;
	SDL_SetTextureColorMod(texture, m_drawColor.r, m_drawColor.g, m_drawColor.b);
	SDL_SetTextureAlphaMod(texture, m_drawColor.a);

	SDL_Rect src;
	src.x = srcx;
	src.y = srcy;
	src.w = srcw;
	src.h = srch;

	Uint8 flip = SDL_FLIP_NONE;
	if (scx < 0) {
		flip |= SDL_FLIP_HORIZONTAL;
		scx *= -1;
	}
	if (scy < 0) {
		flip |= SDL_FLIP_VERTICAL;
		scy *= -1;
	}

	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = (float)srcw * scx;
	dest.h = (float)srch * scy;

	SDL_FPoint center;
	center.x = dest.w * 0.5f;
	center.y = dest.h * 0.5f;

	SDL_RenderCopyExF(m_handle, texture, &src, &dest, angle, &center, (SDL_RendererFlip)flip);
}

void milk::SDLRenderer::present()
{
	SDL_RenderPresent(m_handle);
}
