#include "SDLWindow.h"

#include <iostream>

#include "SDL.h"

milk::SDLWindow::SDLWindow()
	: m_handle{ nullptr }
	, m_shouldClose{ false }
	, m_width{ 800 }
	, m_height{ 600 } { }

bool milk::SDLWindow::init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "Error initializing SDL_Video & SDL_Timer: " << SDL_GetError() << std::endl;
		return false;
	}

	m_handle = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_HIDDEN);
	if (m_handle == nullptr) {
		std::cout << "Error creating SDL_Window: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

std::string milk::SDLWindow::title() const {
	return SDL_GetWindowTitle(m_handle);
}

void milk::SDLWindow::title(const std::string& title) {
	SDL_SetWindowTitle(m_handle, title.c_str());
}

std::tuple<int, int> milk::SDLWindow::dimensions() const {
	int w, h;
	SDL_GetWindowSize(m_handle, &w, &h);
	return std::make_tuple(w, h);
}

void milk::SDLWindow::size(int width, int height) {
	m_width = width;
	m_height = height;
	SDL_SetWindowSize(m_handle, width, height);
	SDL_SetWindowPosition(m_handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

bool milk::SDLWindow::fullscreen() const {
	return (SDL_GetWindowFlags(m_handle) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN;
}

void milk::SDLWindow::fullscreen(const bool toggle) {
	if (toggle == fullscreen()) {
		return;
	}

	if (toggle) {
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

void milk::SDLWindow::show() {
	SDL_ShowWindow(m_handle);
}

void milk::SDLWindow::close() {
	m_shouldClose = true;
}

bool milk::SDLWindow::shouldClose() const {
	return m_shouldClose;
}

SDL_Window* milk::SDLWindow::handle() const {
	return m_handle;
}

void milk::SDLWindow::free() {
	SDL_DestroyWindow(m_handle);
	SDL_Quit();
}
