#include "LuaKeyboard.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "core/Locator.h"
#include "core/lua_extensions.h"

namespace {
	void set_key_enum(lua_State* L) {
		lua_newtable(L);
		milk::luaM::set_int_field(L, "ESC", (int)milk::Keys::ESCAPE);
		milk::luaM::set_int_field(L, "A", (int)milk::Keys::A);
		milk::luaM::set_int_field(L, "B", (int)milk::Keys::B);
		milk::luaM::set_int_field(L, "C", (int)milk::Keys::C);
		milk::luaM::set_int_field(L, "D", (int)milk::Keys::D);
		milk::luaM::set_int_field(L, "E", (int)milk::Keys::E);
		milk::luaM::set_int_field(L, "F", (int)milk::Keys::F);
		milk::luaM::set_int_field(L, "G", (int)milk::Keys::G);
		milk::luaM::set_int_field(L, "H", (int)milk::Keys::H);
		milk::luaM::set_int_field(L, "I", (int)milk::Keys::I);
		milk::luaM::set_int_field(L, "J", (int)milk::Keys::J);
		milk::luaM::set_int_field(L, "K", (int)milk::Keys::K);
		milk::luaM::set_int_field(L, "L", (int)milk::Keys::L);
		milk::luaM::set_int_field(L, "M", (int)milk::Keys::M);
		milk::luaM::set_int_field(L, "N", (int)milk::Keys::N);
		milk::luaM::set_int_field(L, "O", (int)milk::Keys::O);
		milk::luaM::set_int_field(L, "P", (int)milk::Keys::P);
		milk::luaM::set_int_field(L, "Q", (int)milk::Keys::Q);
		milk::luaM::set_int_field(L, "R", (int)milk::Keys::R);
		milk::luaM::set_int_field(L, "S", (int)milk::Keys::S);
		milk::luaM::set_int_field(L, "T", (int)milk::Keys::T);
		milk::luaM::set_int_field(L, "U", (int)milk::Keys::U);
		milk::luaM::set_int_field(L, "V", (int)milk::Keys::V);
		milk::luaM::set_int_field(L, "W", (int)milk::Keys::W);
		milk::luaM::set_int_field(L, "X", (int)milk::Keys::X);
		milk::luaM::set_int_field(L, "Y", (int)milk::Keys::Y);
		milk::luaM::set_int_field(L, "Z", (int)milk::Keys::Z);
		lua_setfield(L, -2, "keys");
	}

	int is_key_down(lua_State* L) {
		int isnum;
		int key = lua_tointegerx(L, 1, &isnum);
		if (isnum) {
			bool down = milk::Locator::keyboard->isKeyDown((milk::Keys)key);
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
			bool pressed = milk::Locator::keyboard->isKeyPressed((milk::Keys)key);
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
			bool released = milk::Locator::keyboard->isKeyReleased((milk::Keys)key);
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

void milk::LuaKeyboard::set_keyboard_submodule(lua_State* L) {
	luaL_newlib(L, lib);
	set_key_enum(L);
	lua_setfield(L, -2, "keyboard");
}
