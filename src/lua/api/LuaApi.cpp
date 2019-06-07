#include "LuaApi.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "LuaWindow.h"
#include "lua/lua_extensions.h"
#include "input/Keyboard.h"

namespace {
	int is_key_down(lua_State* L) {
		const char* key = lua_tostring(L, 1);
		bool pressed = milk::Keyboard::getKeyPressed((SDL_Keycode)key[0]);
		lua_pushboolean(L, pressed);
		return 1;
	}

	static const luaL_Reg lib[] = {
		{ "is_key_down", is_key_down },
		{ NULL, NULL }
	};
}

void milk::LuaApi::registerApi(lua_State* L) {
	lua::register_module(L, "milk", lib);
	LuaWindow::Register(L);
}
