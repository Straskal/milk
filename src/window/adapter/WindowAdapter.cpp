#include "WindowAdapter.h"

#include <iostream>

#include "SDL.h"

#include "RendererAdapter.h"

milk::adapter::WindowAdapter::WindowAdapter()
{
	width_ = 0;
	height_ = 0;
	fullscreen_ = false;
	initialized_ = false;
	sdlWindow_ = nullptr;
}

bool milk::adapter::WindowAdapter::init(const std::string& title, unsigned int width, unsigned int height, bool fullscreen)
{
	if (initialized_)
		return true;

	title_ = title;
	width_ = width;
	height_ = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		std::cout << "Error initializing SDL_Video & SDL_Timer: " << SDL_GetError() << std::endl;
		return false;
	}

	sdlWindow_ = SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_SHOWN);

	if (sdlWindow_ == nullptr)
	{
		std::cout << "Error creating SDL_Window: " << SDL_GetError() << std::endl;
		return false;
	}

	if (fullscreen)
		toggleFullscreen();

	initialized_ = true;

	return true;
}

unsigned int milk::adapter::WindowAdapter::width() const
{
	return width_;
}

unsigned int milk::adapter::WindowAdapter::height() const
{
	return height_;
}

bool milk::adapter::WindowAdapter::fullscreen() const
{
	return fullscreen_;
}

void milk::adapter::WindowAdapter::toggleFullscreen()
{
	fullscreen_ = !fullscreen_;

	if (fullscreen_)
	{
		int displayIndex = SDL_GetWindowDisplayIndex(sdlWindow_);

		SDL_Rect displayBounds;
		SDL_GetDisplayBounds(displayIndex, &displayBounds);

		SDL_SetWindowSize(sdlWindow_, displayBounds.w, displayBounds.h);
		SDL_SetWindowFullscreen(sdlWindow_, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		SDL_SetWindowFullscreen(sdlWindow_, MILK_SDL_WINDOW_NO_FLAG);
		SDL_SetWindowSize(sdlWindow_, width_, height_);
		SDL_SetWindowPosition(sdlWindow_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}

SDL_Window* milk::adapter::WindowAdapter::sdlWindow() const
{
	SDL_assert(sdlWindow_ != nullptr);
	return sdlWindow_;
}

void milk::adapter::WindowAdapter::free()
{
	SDL_DestroyWindow(sdlWindow_);
	SDL_Quit();
}
