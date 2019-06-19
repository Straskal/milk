#include "SDLRenderer.h"

#include <iostream>

#include "SDL_assert.h"
#include "SDL_hints.h"
#include "SDL_render.h"

#include "graphics/Color.h"
#include "graphics/Texture.h"
#include "math/Rectangle.h"

const static int FIRST_SUPPORTED_RENDERING_DRIVER = -1;

milk::SDLRenderer::SDLRenderer()
	: m_handle{ nullptr } { }

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

void milk::SDLRenderer::drawRectangle(const milk::Rectangle* destinationRectangle, const milk::Color* color) {
	SDL_Rect dst = { destinationRectangle->x, destinationRectangle->y, destinationRectangle->width, destinationRectangle->height };
	SDL_SetRenderDrawColor(m_handle, color->r, color->b, color->g, color->a);
	SDL_RenderFillRect(m_handle, &dst);
}

void milk::SDLRenderer::draw(
	const milk::Texture* texture,
	const milk::Rectangle* sourceRectangle,
	const milk::Rectangle* destinationRectangle,
	milk::u8 flipFlags
) {
	SDL_Rect src = { sourceRectangle->x, sourceRectangle->y, sourceRectangle->width, sourceRectangle->height };
	SDL_Rect dst = { destinationRectangle->x, destinationRectangle->y, destinationRectangle->width, destinationRectangle->height };
	SDL_RenderCopyEx(m_handle, (SDL_Texture*)texture->data->handle, &src, &dst, 0, nullptr, (SDL_RendererFlip)flipFlags);
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
