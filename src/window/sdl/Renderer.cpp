#include "Renderer.h"

#include <iostream>

#include "SDL_assert.h"
#include "SDL_hints.h"
#include "SDL_render.h"

#include "graphics/Color.h"
#include "graphics/Texture.h"

#include "math/Rectangle.h"

const static int FIRST_SUPPORTED_RENDERING_DRIVER = -1;

milk::sdl::Renderer::Renderer()
	: m_resolution{ 0, 0 }
	, m_handle{ nullptr }{ }

bool milk::sdl::Renderer::init(SDL_Window* windowHandle) {
	SDL_GetWindowSize(windowHandle, &m_resolution.width, &m_resolution.height);
	m_handle = SDL_CreateRenderer(windowHandle, FIRST_SUPPORTED_RENDERING_DRIVER, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (m_handle == nullptr) {
		std::cout << "Error creating SDL_Renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(m_handle, m_resolution.width, m_resolution.height);
	SDL_SetRenderDrawBlendMode(m_handle, SDL_BLENDMODE_BLEND);
	return true;
}

void milk::sdl::Renderer::clear(const Color& color) {
	SDL_SetRenderDrawColor(m_handle, color.r, color.b, color.g, color.a);
	SDL_RenderClear(m_handle);
}

void milk::sdl::Renderer::drawRectangle(const milk::Rectangle& destinationRectangle, const milk::Color& color) {
	SDL_Rect dst = { destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height };
	SDL_SetRenderDrawColor(m_handle, color.r, color.b, color.g, color.a);
	SDL_RenderFillRect(m_handle, &dst);
}

void milk::sdl::Renderer::draw(
	const milk::Texture& texture,
	const milk::Rectangle& sourceRectangle,
	const milk::Rectangle& destinationRectangle,
	milk::u8 flipFlags
) {
	SDL_Rect src = { sourceRectangle.x, sourceRectangle.y, sourceRectangle.width, sourceRectangle.height };
	SDL_Rect dst = { destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height };
	SDL_RenderCopyEx(m_handle, texture.get(), &src, &dst, 0, nullptr, (SDL_RendererFlip)flipFlags);
}

void milk::sdl::Renderer::present() {
	SDL_RenderPresent(m_handle);
}

milk::Resolution milk::sdl::Renderer::resolution() const {
	return m_resolution;
}

void milk::sdl::Renderer::resolution(int w, int h) {
	m_resolution.width = w;
	m_resolution.height = h;
	SDL_RenderSetLogicalSize(m_handle, w, h);
}

void milk::sdl::Renderer::free() {
	SDL_DestroyRenderer(m_handle);
}
