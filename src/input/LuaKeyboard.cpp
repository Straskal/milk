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
			bool pressed = milk::Locator::keyboard->isKeyPressed((SDL_Keycode)key);
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
	luaM::set_int_field(L, "ESC", (int)Keys::ESCAPE);
	luaM::set_int_field(L, "A", (int)Keys::A);
	luaM::set_int_field(L, "B", (int)Keys::B);
	luaM::set_int_field(L, "C", (int)Keys::C);
	luaM::set_int_field(L, "D", (int)Keys::D);
	luaM::set_int_field(L, "E", (int)Keys::E);
	luaM::set_int_field(L, "F", (int)Keys::F);
	luaM::set_int_field(L, "G", (int)Keys::G);
	luaM::set_int_field(L, "H", (int)Keys::H);
	luaM::set_int_field(L, "I", (int)Keys::I);
	luaM::set_int_field(L, "J", (int)Keys::J);
	luaM::set_int_field(L, "K", (int)Keys::K);
	luaM::set_int_field(L, "L", (int)Keys::L);
	luaM::set_int_field(L, "M", (int)Keys::M);
	luaM::set_int_field(L, "N", (int)Keys::N);
	luaM::set_int_field(L, "O", (int)Keys::O);
	luaM::set_int_field(L, "P", (int)Keys::P);
	luaM::set_int_field(L, "Q", (int)Keys::Q);
	luaM::set_int_field(L, "R", (int)Keys::R);
	luaM::set_int_field(L, "S", (int)Keys::S);
	luaM::set_int_field(L, "T", (int)Keys::T);
	luaM::set_int_field(L, "U", (int)Keys::U);
	luaM::set_int_field(L, "V", (int)Keys::V);
	luaM::set_int_field(L, "W", (int)Keys::W);
	luaM::set_int_field(L, "X", (int)Keys::X);
	luaM::set_int_field(L, "Y", (int)Keys::Y);
	luaM::set_int_field(L, "Z", (int)Keys::Z);
	lua_setfield(L, -2, "KEYS");

	lua_setfield(L, -2, "milk.keyboard");

	// Pop global table off of stack
	lua_pop(L, 1);
}
