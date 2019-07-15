#include "window.h"

#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

#include "keyboard/keyboard.h"
#include "mouse/mouse.h"
#include "joystick/joystick.h"
#include "controller/controller.h"

/* Window */
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

/* We define this bad boi here because SDL does not, and we don't want magic numbers chilling up in this boi, son. */
#define MILK_SDL_WINDOW_NO_FLAG 0

static SDL_Window* window_handle = nullptr;
static bool should_close = false;

bool milk::window_init()
{
	should_close = false;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "Error initializing SDL_Video & SDL_Timer: " << SDL_GetError() << std::endl;
		return false;
	}
	/*
		We create a hidden window that must be explicitly shown via show()
		This is done in order to give lua a chance to change the window settings before showing it.
	*/
	window_handle = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_HIDDEN);
	if (window_handle == nullptr) {
		std::cout << "Error creating SDL_Window: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

void milk::window_quit()
{
	SDL_DestroyWindow(window_handle);
	SDL_Quit();
}

void* milk::window_get_handle()
{
	return window_handle;
}

void milk::window_poll()
{
	SDL_Event event;

	milk::mouse_reset_scroll();
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			milk::window_close();
		if (event.type == SDL_MOUSEWHEEL)
			milk::mouse_handle_wheel_event(&event);
		if (event.type == SDL_JOYAXISMOTION)
			milk::joystick_handle_axis_event(&event);
		if (event.type == SDL_CONTROLLERAXISMOTION)
			milk::controller_handle_axis_event();
	}

	milk::mouse_update_state();
	milk::keyboard_update_state();
	milk::controller_update_state();
}

void milk::window_show()
{
	SDL_ShowWindow(window_handle);
	SDL_RaiseWindow(window_handle);
}

const char* milk::window_get_title()
{
	return SDL_GetWindowTitle(window_handle);
}

void milk::window_set_title(const char* title)
{
	SDL_SetWindowTitle(window_handle, title);
}

void milk::window_set_icon(const char* filepath)
{
	/*
		I am not in love with this solution. I'd really like for SDL_image to stay in the graphics portion of milk.
		Unfortuantely, SDL_SetWindowIcon only acceps SDL_Surfaces, so the only alternate solution would be for milk::Images
		to keep a reference to the SDL_Texture AND surface, but that's really unnecessary as this is the only need for it.
	*/
	SDL_Surface* icon = IMG_Load(filepath);
	if (icon == nullptr) {
		std::cout << "Error loading image: " << IMG_GetError() << std::endl;
		return;
	}
	SDL_SetWindowIcon(window_handle, icon);
	SDL_FreeSurface(icon);
}

std::tuple<int, int> milk::window_get_size()
{
	int w, h;
	SDL_GetWindowSize(window_handle, &w, &h);
	return std::make_tuple(w, h);
}

void milk::window_set_size(int width, int height)
{
	SDL_SetWindowSize(window_handle, width, height);
	SDL_SetWindowPosition(window_handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

bool milk::window_get_is_fullscreen()
{
	return (SDL_GetWindowFlags(window_handle) & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP;
}

void milk::window_set_is_fullscreen(bool toggle)
{
	if (toggle != window_get_is_fullscreen()) {
		if (toggle)
			SDL_SetWindowFullscreen(window_handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else
			SDL_SetWindowFullscreen(window_handle, MILK_SDL_WINDOW_NO_FLAG);
	}
}

void milk::window_minimize()
{
	SDL_MinimizeWindow(window_handle);
}

void milk::window_restore()
{
	SDL_RestoreWindow(window_handle);
}

void milk::window_close()
{
	should_close = true;
}

bool milk::window_should_close()
{
	return should_close;
}
