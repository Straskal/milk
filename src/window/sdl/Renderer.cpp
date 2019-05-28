#include "Renderer.h"

#include <iostream>

#include "SDL_assert.h"
#include "SDL_hints.h"
#include "SDL_render.h"

#include "graphics/Color.h"
#include "graphics/Texture.h"

#include "math/Rectangle.h"

milk::sdl::Renderer::Renderer() {
	initialized_ = false;
	resolution_.width = 0;
	resolution_.height = 0;
	handle_ = nullptr;
}

bool milk::sdl::Renderer::init(void* windowHandle, int resolutionWidth, int resolutionHeight) {
	if (initialized_) {
		return true;
	}
	resolution_.width = resolutionWidth;
	resolution_.height = resolutionHeight;
	handle_ = SDL_CreateRenderer((SDL_Window*)windowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (handle_ == nullptr) {
		std::cout << "Error creating SDL_Renderer: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(handle_, resolutionWidth, resolutionHeight);
	SDL_SetRenderDrawBlendMode(handle_, SDL_BLENDMODE_BLEND);
	initialized_ = true;
	return true;
}

void milk::sdl::Renderer::clear(const Color& color) {
	SDL_SetRenderDrawColor(handle_, color.red, color.blue, color.green, color.alpha);
	SDL_RenderClear(handle_);
}

void milk::sdl::Renderer::drawRectangle(const milk::Rectangle& destinationRectangle, const milk::Color& color) {
	SDL_Rect dst = { destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height };

	SDL_SetRenderDrawColor(handle_, color.red, color.blue, color.green, color.alpha);
	SDL_RenderFillRect(handle_, &dst);
}

void milk::sdl::Renderer::draw(
	const milk::Texture& texture,
	const milk::Rectangle& sourceRectangle,
	const milk::Rectangle& destinationRectangle,
	milk::U8 flipFlags) {
	SDL_Rect src = { sourceRectangle.x, sourceRectangle.y, sourceRectangle.width, sourceRectangle.height };
	SDL_Rect dst = { destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height };

	SDL_RenderCopyEx(handle_, texture.get(), &src, &dst, 0, nullptr, (SDL_RendererFlip)flipFlags);
}

void milk::sdl::Renderer::present() {
	SDL_RenderPresent(handle_);
}

milk::Resolution milk::sdl::Renderer::resolution() const {
	return resolution_;
}

void milk::sdl::Renderer::free() {
	SDL_DestroyRenderer(handle_);
}
