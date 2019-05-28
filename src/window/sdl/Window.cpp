#include "Window.h"
#include "Window.h"

#include <iostream>

#include "SDL.h"

#include "Renderer.h"

milk::sdl::Window::Window() 
	: handle_{ nullptr }
	, width_{ 0 }
	, height_{ 0 } {}

bool milk::sdl::Window::init(const std::string& title, int width,  int height, bool fullscreen) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "Error initializing SDL_Video & SDL_Timer: " << SDL_GetError() << std::endl;
		return false;
	}

	width_ = width;
	height_ = height;

	handle_ = SDL_CreateWindow(
		title.c_str(), 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 
		width, 
		height, 
		SDL_WINDOW_SHOWN
	);

	if (handle_ == nullptr) {
		std::cout << "Error creating SDL_Window: " << SDL_GetError() << std::endl;
		return false;
	}
	if (fullscreen) {
		toggleFullscreen();
	}
	return true;
}

bool milk::sdl::Window::fullscreen() const {
	Uint32 flags = SDL_GetWindowFlags(handle_);
	return (flags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN;
}

void milk::sdl::Window::toggleFullscreen() {
	if (!fullscreen()) {
		int displayIndex = SDL_GetWindowDisplayIndex(handle_);
		SDL_Rect displayBounds;
		SDL_GetDisplayBounds(displayIndex, &displayBounds);
		SDL_SetWindowSize(handle_, displayBounds.w, displayBounds.h);
		SDL_SetWindowFullscreen(handle_, SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(handle_, MILK_SDL_WINDOW_NO_FLAG);
		SDL_SetWindowSize(handle_, width_, height_);
		SDL_SetWindowPosition(handle_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}

void* milk::sdl::Window::handle() const {
	SDL_assert(handle_ != nullptr);
	return handle_;
}

void milk::sdl::Window::free() {
	SDL_DestroyWindow(handle_);
	SDL_Quit();
}
