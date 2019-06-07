#include "LuaKeyboard.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "core/Locator.h"
#include "lua/lua_extensions.h"

namespace {
	int is_key_down(lua_State* L) {
		const char* key = lua_tostring(L, 1);
		bool pressed = milk::Locator::keyboard->isKeyPressed((SDL_Keycode)key[0]);
		lua_pushboolean(L, pressed);
		return 1;
	}

	static const luaL_Reg lib[] = {
		{ "is_key_down", is_key_down },
		{ NULL, NULL }
	};
}

void milk::LuaKeyboard::bind(lua_State* L) {
	lua::register_module(L, "milk.keyboard", lib);
}
