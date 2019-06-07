#include "LuaWindow.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "core/Locator.h"
#include "lua/lua_extensions.h"
#include "window/Window.h"

namespace {
	int set_fullscreen(lua_State* L) {
		bool fs = lua_toboolean(L, 1);
		milk::Locator::window->fullscreen(fs);
		return 0;
	}

	int is_fullscreen(lua_State* L) {
		bool fs = milk::Locator::window->fullscreen();
		lua_pushboolean(L, fs);
		return 1;
	}

	static const luaL_Reg lib[] = {
		{ "set_fullscreen", set_fullscreen },
		{ "is_fullscreen", is_fullscreen },
		{ NULL, NULL }
	};
}

void milk::LuaWindow::bind(lua_State* L) {
	lua::register_module(L, "milk.window", lib);
}
