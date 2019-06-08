#include "LuaKeyboard.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "core/Locator.h"
#include "lua/lua_extensions.h"

namespace {
	int is_key_down(lua_State* L) {
		int isnum;
		int key = lua_tointegerx(L, 1, &isnum);
		if (isnum) {
			bool down = milk::Locator::keyboard->isKeyDown((SDL_Keycode)key);
			lua_pushboolean(L, down);
			return 1;
		}
		lua_pushboolean(L, false);
		return 1;
	}

	int is_key_pressed(lua_State* L) {
		int isnum;
		int key = lua_tointegerx(L, 1, &isnum);
		if (isnum) {
			bool pressed = milk::Locator::keyboard->isKeyReleased((SDL_Keycode)key);
			lua_pushboolean(L, pressed);
			return 1;
		}
		lua_pushboolean(L, false);
		return 1;
	}

	int is_key_released (lua_State* L) {
		int isnum;
		int key = lua_tointegerx(L, 1, &isnum);
		if (isnum) {
			bool released = milk::Locator::keyboard->isKeyReleased((SDL_Keycode)key);
			lua_pushboolean(L, released);
			return 1;
		}
		lua_pushboolean(L, false);
		return 1;
	}

	const luaL_Reg lib[] = {
		{ "is_key_down", is_key_down },
		{ "is_key_pressed", is_key_pressed },
		{ "is_key_released", is_key_released },
		{ NULL, NULL }
	};
}

void milk::LuaKeyboard::bind(lua_State* L) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");
	luaL_newlib(L, lib);

	// Create milk.keyboard.KEYS table
	lua_newtable(L);
	lua::set_int_field(L, "ESC", (int)Keys::ESCAPE);
	lua::set_int_field(L, "A", (int)Keys::A);
	lua::set_int_field(L, "B", (int)Keys::B);
	lua::set_int_field(L, "C", (int)Keys::C);
	lua::set_int_field(L, "D", (int)Keys::D);
	lua::set_int_field(L, "E", (int)Keys::E);
	lua::set_int_field(L, "F", (int)Keys::F);
	lua::set_int_field(L, "G", (int)Keys::G);
	lua::set_int_field(L, "H", (int)Keys::H);
	lua::set_int_field(L, "I", (int)Keys::I);
	lua::set_int_field(L, "J", (int)Keys::J);
	lua::set_int_field(L, "K", (int)Keys::K);
	lua::set_int_field(L, "L", (int)Keys::L);
	lua::set_int_field(L, "M", (int)Keys::M);
	lua::set_int_field(L, "N", (int)Keys::N);
	lua::set_int_field(L, "O", (int)Keys::O);
	lua::set_int_field(L, "P", (int)Keys::P);
	lua::set_int_field(L, "Q", (int)Keys::Q);
	lua::set_int_field(L, "R", (int)Keys::R);
	lua::set_int_field(L, "S", (int)Keys::S);
	lua::set_int_field(L, "T", (int)Keys::T);
	lua::set_int_field(L, "U", (int)Keys::U);
	lua::set_int_field(L, "V", (int)Keys::V);
	lua::set_int_field(L, "W", (int)Keys::W);
	lua::set_int_field(L, "X", (int)Keys::X);
	lua::set_int_field(L, "Y", (int)Keys::Y);
	lua::set_int_field(L, "Z", (int)Keys::Z);
	lua_setfield(L, -2, "KEYS");

	lua_setfield(L, -2, "milk.keyboard");

	// Pop global table off of stack
	lua_pop(L, 1);
}
