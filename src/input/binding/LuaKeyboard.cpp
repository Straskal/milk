#include "LuaKeyboard.h"

#include "SDL_keycode.h"

#include "scripting/sol/sol.hpp"

#include "input/Keyboard.h"

void milk::lua::LuaKeyboard::bind(sol::state& luaState)
{
	luaState.new_enum("Keys",
		"A", SDLK_a,
		"D", SDLK_d,
		"F", SDLK_f,
		"R", SDLK_r,
		"S", SDLK_s,
		"W", SDLK_w,
		"Space", SDLK_SPACE,
		"Tilde", SDLK_BACKQUOTE);

	luaState.new_usertype<Keyboard>("Keyboard",
		"get_key", &Keyboard::getKey,
		"get_key_pressed", &Keyboard::getKeyPressed,
		"get_key_released", &Keyboard::getKeyReleased);
}
