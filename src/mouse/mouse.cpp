#include "mouse.h"

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
	return std::make_tuple(x, y);
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
	return !(current_state & SDL_BUTTON((int)button) && (previous_state & SDL_BUTTON((int)button)));
}

int milk::mouse_get_scroll()
{
	return scroll;
}
