#include "SDLWindow.h"

#include <iostream>

#include "SDL.h"

static const int DEFAULT_WIDTH = 800;
static const int DEFAULT_HEIGHT = 600;

// We define this bad boi here because SDL does not, and we don't want magic numbers chilling up in this boi, son.
static const int MILK_SDL_WINDOW_NO_FLAG = 0;

milk::SDLWindow::SDLWindow()
	: m_handle{ nullptr }
	, m_shouldClose{ false } { }

bool milk::SDLWindow::init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "Error initializing SDL_Video & SDL_Timer: " << SDL_GetError() << std::endl;
		return false;
	}

	// We create a hidden window that must be explicitly shown via show()
	// This is done in order to give lua a chance to change the window settings before showing it.
	m_handle = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_HIDDEN);
	if (m_handle == nullptr) {
		std::cout << "Error creating SDL_Window: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

const char* milk::SDLWindow::title() const
{
	return SDL_GetWindowTitle(m_handle);
}

void milk::SDLWindow::title(const char* title)
{
	SDL_SetWindowTitle(m_handle, title);
}

std::tuple<int, int> milk::SDLWindow::size() const
{
	int w, h;
	SDL_GetWindowSize(m_handle, &w, &h);
	return std::make_tuple(w, h);
}

void milk::SDLWindow::size(int width, int height)
{
	SDL_SetWindowSize(m_handle, width, height);
	SDL_SetWindowPosition(m_handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

bool milk::SDLWindow::fullscreen() const
{
	return (SDL_GetWindowFlags(m_handle) & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP;
}

void milk::SDLWindow::fullscreen(const bool toggle)
{
	if (toggle != fullscreen()) {
		if (toggle) {
			SDL_SetWindowFullscreen(m_handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else {
			SDL_SetWindowFullscreen(m_handle, MILK_SDL_WINDOW_NO_FLAG);
		}
	}
}

void milk::SDLWindow::minimize()
{
	SDL_MinimizeWindow(m_handle);
}

void milk::SDLWindow::restore()
{
	SDL_RestoreWindow(m_handle);
}

void milk::SDLWindow::show()
{
	SDL_ShowWindow(m_handle);
}

void milk::SDLWindow::close()
{
	m_shouldClose = true;
}

bool milk::SDLWindow::shouldClose() const
{
	return m_shouldClose;
}

SDL_Window* milk::SDLWindow::handle() const
{
	return m_handle;
}

void milk::SDLWindow::free()
{
	SDL_DestroyWindow(m_handle);
	SDL_Quit();
}
