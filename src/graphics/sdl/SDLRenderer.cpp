#include "SDLRenderer.h"

#include <iostream>

#include "SDL_assert.h"
#include "SDL_hints.h"
#include "SDL_render.h"

#include "graphics/Color.h"
#include "graphics/Texture.h"
#include "math/Rectangle.h"

const static int FIRST_SUPPORTED_RENDERING_DRIVER = -1;

static void milkrect_to_sdlrect(const milk::Rectangle* milkrect, SDL_Rect* sdlrect) {
	sdlrect->x = milkrect->x;
	sdlrect->y = milkrect->y;
	sdlrect->w = milkrect->width;
	sdlrect->h = milkrect->height;
}

milk::SDLRenderer::SDLRenderer()
	: m_handle{ nullptr }
	, m_sourceRect{ 0, 0, 0, 0 }
	, m_destRect{ 0, 0, 0, 0 } {}

bool milk::SDLRenderer::init(SDL_Window* windowHandle) {
	m_handle = SDL_CreateRenderer(windowHandle, FIRST_SUPPORTED_RENDERING_DRIVER, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_handle == nullptr) {
		std::cout << "Error creating SDL_Renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	// Default the resolution to the window dimensions.
	int w, h;
	SDL_GetWindowSize(windowHandle, &w, &h);
	SDL_RenderSetLogicalSize(m_handle, w, h);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_SetRenderDrawBlendMode(m_handle, SDL_BLENDMODE_BLEND);
	return true;
}

void milk::SDLRenderer::clear() {
	SDL_SetRenderDrawColor(m_handle, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(m_handle);
}

void milk::SDLRenderer::setDrawColor(const Color* color) {
	SDL_SetRenderDrawColor(m_handle, color->r, color->b, color->g, color->a);
}

void milk::SDLRenderer::drawRectangle(const Rectangle* destinationRectangle) {
	milkrect_to_sdlrect(destinationRectangle, &m_destRect);
	SDL_RenderDrawRect(m_handle, &m_destRect);
}

void milk::SDLRenderer::drawRectangleFilled(const milk::Rectangle* destinationRectangle) {
	milkrect_to_sdlrect(destinationRectangle, &m_destRect);
	SDL_RenderFillRect(m_handle, &m_destRect);
}

void milk::SDLRenderer::draw(
	const milk::Texture* texture,
	const milk::Rectangle* sourceRectangle,
	const milk::Rectangle* destinationRectangle,
	milk::u8 flipFlags
) {
	Uint8 r, b, g;
	SDL_GetRenderDrawColor(m_handle, &r, &g, &b, NULL);
	SDL_Texture* t = (SDL_Texture*)texture->data->handle;
	SDL_SetTextureColorMod(t, r, g, b);
	milkrect_to_sdlrect(sourceRectangle, &m_sourceRect);
	milkrect_to_sdlrect(destinationRectangle, &m_destRect);
	SDL_RenderCopyEx(m_handle, t, &m_sourceRect, &m_destRect, 0, nullptr, (SDL_RendererFlip)flipFlags);
}

void milk::SDLRenderer::present() {
	SDL_RenderPresent(m_handle);
}

std::tuple<int, int> milk::SDLRenderer::resolution() const {
	int w, h;
	SDL_RenderGetLogicalSize(m_handle, &w, &h);
	return std::make_tuple(w, h);
}

void milk::SDLRenderer::resolution(int w, int h) {
	SDL_RenderSetLogicalSize(m_handle, w, h);
}

SDL_Renderer* milk::SDLRenderer::handle() const {
	return m_handle;
}

void milk::SDLRenderer::free() {
	SDL_DestroyRenderer(m_handle);
}
