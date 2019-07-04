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
	, m_drawColor{ 0xFF, 0xFF, 0xFF, 0xFF }
	, m_sourceRect{ 0, 0, 0, 0 }
	, m_destRect{ 0, 0, 0, 0 } {}

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

void milk::SDLRenderer::resolution(const int w, const int h)
{
	SDL_RenderSetLogicalSize(m_handle, w, h);
}

void milk::SDLRenderer::clear()
{
	SDL_SetRenderDrawColor(m_handle, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(m_handle);
	SDL_SetRenderDrawColor(m_handle, m_drawColor.r, m_drawColor.b, m_drawColor.g, m_drawColor.a);
}

void milk::SDLRenderer::setDrawColor(const double r, const double g, const double b, const double a)
{
	m_drawColor.r = (Uint8)(std::min((int)(r * 0xFF), 0xFF));
	m_drawColor.g = (Uint8)(std::min((int)(g * 0xFF), 0xFF));
	m_drawColor.b = (Uint8)(std::min((int)(b * 0xFF), 0xFF));
	m_drawColor.a = (Uint8)(std::min((int)(a * 0xFF), 0xFF));
	SDL_SetRenderDrawColor(m_handle, m_drawColor.r, m_drawColor.b, m_drawColor.g, m_drawColor.a);
}

void milk::SDLRenderer::drawRectangle(const float x, const float y, const float w, const float h)
{
	m_destRect.x = x;
	m_destRect.y = y;
	m_destRect.w = w;
	m_destRect.h = h;
	SDL_RenderDrawRectF(m_handle, &m_destRect);
}

void milk::SDLRenderer::drawRectangleFilled(const float x, const float y, const float w, const float h)
{
	m_destRect.x = x;
	m_destRect.y = y;
	m_destRect.w = w;
	m_destRect.h = h;
	SDL_RenderFillRectF(m_handle, &m_destRect);
}

void milk::SDLRenderer::draw(const Image* image, const float x, const float y)
{
	SDL_Texture* texture = (SDL_Texture*)image->data->handle;
	SDL_SetTextureColorMod(texture, m_drawColor.r, m_drawColor.g, m_drawColor.b);
	SDL_SetTextureAlphaMod(texture, m_drawColor.a);

	int w = image->data->width;
	int h = image->data->height;

	m_sourceRect.x = 0;
	m_sourceRect.y = 0;
	m_sourceRect.w = w;
	m_sourceRect.h = h;

	m_destRect.x = x;
	m_destRect.y = y;
	m_destRect.w = (float)w;
	m_destRect.h = (float)h;

	SDL_RenderCopyExF(m_handle, texture, &m_sourceRect, &m_destRect, 0, nullptr, SDL_FLIP_NONE);
}

void milk::SDLRenderer::draw(
	const Image* image,
	const float x, const float y,
	const int srcx, const int srcy, const int srcw, const int srch,
	const float scalex, const float scaley, const double angle)
{
	SDL_Texture* texture = (SDL_Texture*)image->data->handle;
	SDL_SetTextureColorMod(texture, m_drawColor.r, m_drawColor.g, m_drawColor.b);
	SDL_SetTextureAlphaMod(texture, m_drawColor.a);

	m_sourceRect.x = srcx;
	m_sourceRect.y = srcy;
	m_sourceRect.w = srcw;
	m_sourceRect.h = srch;

	float sx = std::min(std::max(scalex, 0.f), 1.f);
	float sy = std::min(std::max(scaley, 0.f), 1.f);
	Uint8 flip = SDL_FLIP_NONE;

	if (sx < 0) {
		flip |= SDL_FLIP_HORIZONTAL;
		sx *= -1;
	}
	if (sy < 0) {
		flip |= SDL_FLIP_VERTICAL;
		sy *= -1;
	}

	m_destRect.x = x;
	m_destRect.y = y;
	m_destRect.w = (float)srcw * sx;
	m_destRect.h = (float)srch * sy;

	SDL_FPoint center;
	center.x = m_destRect.w * 0.5f;
	center.y = m_destRect.h * 0.5f;

	SDL_RenderCopyExF(m_handle, texture, &m_sourceRect, &m_destRect, angle, &center, (SDL_RendererFlip)flip);
}

void milk::SDLRenderer::present()
{
	SDL_RenderPresent(m_handle);
}
