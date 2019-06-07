#include "Window.h"
#include "Window.h"

#include <iostream>

#include "SDL.h"

#include "Renderer.h"

milk::sdl::Window::Window() 
	: m_handle{ nullptr }
	, m_width{ 0 }
	, m_height{ 0 } { }

bool milk::sdl::Window::init(const std::string& title, int width,  int height, bool fs) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "Error initializing SDL_Video & SDL_Timer: " << SDL_GetError() << std::endl;
		return false;
	}

	m_width = width;
	m_height = height;
	m_handle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

	if (m_handle == nullptr) {
		std::cout << "Error creating SDL_Window: " << SDL_GetError() << std::endl;
		return false;
	}

	fullscreen(fs);
	return true;
}

bool milk::sdl::Window::fullscreen() const {
	return (SDL_GetWindowFlags(m_handle) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN;
}

void milk::sdl::Window::fullscreen(bool fs) {
	if (fs == fullscreen()) {
		return;
	}

	if (fs) {
		int displayIndex = SDL_GetWindowDisplayIndex(m_handle);
		SDL_Rect displayBounds;
		SDL_GetDisplayBounds(displayIndex, &displayBounds);
		SDL_SetWindowSize(m_handle, displayBounds.w, displayBounds.h);
		SDL_SetWindowFullscreen(m_handle, SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(m_handle, MILK_SDL_WINDOW_NO_FLAG);
		SDL_SetWindowSize(m_handle, m_width, m_height);
		SDL_SetWindowPosition(m_handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}

SDL_Window* milk::sdl::Window::handle() const {
	return m_handle;
}

void milk::sdl::Window::free() {
	SDL_DestroyWindow(m_handle);
	SDL_Quit();
}
