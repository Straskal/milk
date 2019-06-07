#include "LuaApi.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "main/Locator.h"
#include "input/Keyboard.h"

namespace {
	int toggle_fullscreen(lua_State* L) {
		milk::Locator::window->toggleFullscreen();
		return 0;
	}

	int is_key_down(lua_State* L) {
		const char* key = lua_tostring(L, 1);
		bool pressed = milk::Keyboard::getKeyPressed((SDL_Keycode)key[0]);
		lua_pushboolean(L, pressed);
		return 1;
	}

	static const luaL_Reg lib[] = {
		{ "toggle_fullscreen", toggle_fullscreen },
		{ "is_key_down", is_key_down },
		{ NULL, NULL }
	};
}

void milk::LuaApi::registerApi(lua_State* L) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");
	luaL_newlib(L, lib);
	lua_setfield(L, -2, "milk");
}
