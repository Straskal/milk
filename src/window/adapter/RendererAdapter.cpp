#include "RendererAdapter.h"

#include <iostream>

#include "SDL_assert.h"
#include "SDL_hints.h"
#include "SDL_render.h"

#include "graphics/Color.h"
#include "graphics/Texture.h"

#include "math/Rectangle.h"

milk::adapter::RendererAdapter::RendererAdapter() {
	initialized_ = false;
	resolution_.width = 0;
	resolution_.height = 0;
}

bool milk::adapter::RendererAdapter::init(SDL_Window* sdlWindow, unsigned int resolutionWidth, unsigned int resolutionHeight) {
	if (initialized_) {
		return true;
	}
	resolution_.width = resolutionWidth;
	resolution_.height = resolutionHeight;
	sdlRenderer_ = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (sdlRenderer_ == nullptr) {
		std::cout << "Error creating SDL_Renderer: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(sdlRenderer_, resolutionWidth, resolutionHeight);
	SDL_SetRenderDrawBlendMode(sdlRenderer_, SDL_BLENDMODE_BLEND);
	initialized_ = true;
	return true;
}

void milk::adapter::RendererAdapter::clear(const Color& color) {
	SDL_SetRenderDrawColor(sdlRenderer_, color.red, color.blue, color.green, color.alpha);
	SDL_RenderClear(sdlRenderer_);
}

void milk::adapter::RendererAdapter::drawRectangle(const milk::Rectangle& destinationRectangle, const milk::Color& color) {
	SDL_Rect dst = { destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height };

	SDL_SetRenderDrawColor(sdlRenderer_, color.red, color.blue, color.green, color.alpha);
	SDL_RenderFillRect(sdlRenderer_, &dst);
}

void milk::adapter::RendererAdapter::drawRectangleOutline(const milk::Rectangle& destinationRectangle, const milk::Color& color) {
	SDL_Rect dst = { destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height };

	SDL_SetRenderDrawColor(sdlRenderer_, color.red, color.blue, color.green, color.alpha);
	SDL_RenderDrawRect(sdlRenderer_, &dst);
}

void milk::adapter::RendererAdapter::draw(
	const milk::Texture& texture,
	const milk::Rectangle& sourceRectangle,
	const milk::Rectangle& destinationRectangle,
	milk::U8 flipFlags) {
	SDL_Rect src = { sourceRectangle.x, sourceRectangle.y, sourceRectangle.width, sourceRectangle.height };
	SDL_Rect dst = { destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height };

	SDL_RenderCopyEx(sdlRenderer_, texture.get(), &src, &dst, 0, nullptr, (SDL_RendererFlip)flipFlags);
}

void milk::adapter::RendererAdapter::present() {
	SDL_RenderPresent(sdlRenderer_);
}

milk::Resolution milk::adapter::RendererAdapter::resolution() const {
	return resolution_;
}

SDL_Renderer* milk::adapter::RendererAdapter::sdlRenderer() const {
	SDL_assert(sdlRenderer_ != nullptr);
	return sdlRenderer_;
}

void milk::adapter::RendererAdapter::free() {
	SDL_DestroyRenderer(sdlRenderer_);
}
