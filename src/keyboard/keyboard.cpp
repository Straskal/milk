#include "keyboard.h"

#include <iostream>
#include <cstring>

#include "SDL.h"

static Uint8 current_state[SDL_NUM_SCANCODES];
static Uint8 previous_state[SDL_NUM_SCANCODES];

bool milk::keyboard_update_state()
{
	std::memcpy(previous_state, current_state, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(current_state, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

bool milk::keyboard_is_key_down(Keys key)
{
	return current_state[(SDL_Scancode)key];
}

bool milk::keyboard_is_key_pressed(Keys key)
{
	return current_state[(SDL_Scancode)key] && !previous_state[(SDL_Scancode)key];
}

bool milk::keyboard_is_key_released(Keys key)
{
	return !current_state[(SDL_Scancode)key] && previous_state[(SDL_Scancode)key];
}
