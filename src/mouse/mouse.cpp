#include "mouse.h"

#include "window/window.h"
#include "graphics/graphics.h"

static Uint32 current_state;
static Uint32 previous_state;
static int x;
static int y;
static int scroll;

void milk::mouse_reset_scroll()
{
	scroll = 0;
}

void milk::mouse_handle_wheel_event(SDL_Event* e)
{
	scroll = e->wheel.y;
}

void milk::mouse_update_state()
{
	previous_state = current_state;
	current_state = SDL_GetMouseState(&x, &y);
}

std::tuple<int, int> milk::mouse_get_position()
{
	/*
		SDL's logical resolution filter only applies to events pumped through the event loop, not the real time state updates.
		This means that we have to handle it ourselves.
	*/
	std::tuple<int, int> winsize = milk::window_get_size();
	std::tuple<int, int> resolution = milk::graphics_get_resolution();

	float normMousex = (float)x / std::get<0>(winsize);
	float normMouseY = (float)y / std::get<1>(winsize);
	int mousex = (int)(normMousex * std::get<0>(resolution));
	int mousey = (int)(normMouseY * std::get<1>(resolution));

	return std::make_tuple(mousex, mousey);
}

bool milk::mouse_is_button_down(MouseButtons button)
{
	return current_state & SDL_BUTTON((int)button);
}

bool milk::mouse_is_button_pressed(MouseButtons button)
{
	return current_state & SDL_BUTTON((int)button) && !(previous_state & SDL_BUTTON((int)button));
}

bool milk::mouse_is_button_released(MouseButtons button)
{
	return !(current_state & SDL_BUTTON((int)button)) && (previous_state & SDL_BUTTON((int)button));
}

int milk::mouse_get_scroll()
{
	return scroll;
}
