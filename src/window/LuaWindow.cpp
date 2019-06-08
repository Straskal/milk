#include "LuaWindow.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Window.h"
#include "core/Locator.h"
#include "lua/lua_extensions.h"

namespace {
	int set_fullscreen(lua_State* L) {
		if (lua_isboolean(L, 1)) {
			bool toggle = lua_toboolean(L, 1);
			milk::Locator::window->fullscreen(toggle);
		}		
		return 0;
	}

	int is_fullscreen(lua_State* L) {
		bool toggle = milk::Locator::window->fullscreen();
		lua_pushboolean(L, toggle);
		return 1;
	}

	static const luaL_Reg lib[] = {
		{ "set_fullscreen", set_fullscreen },
		{ "is_fullscreen", is_fullscreen },
		{ NULL, NULL }
	};
}

void milk::LuaWindow::bind(lua_State* L) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");
	luaL_newlib(L, lib);
	lua_setfield(L, -2, "milk.window");
	lua_pop(L, 1);
}
