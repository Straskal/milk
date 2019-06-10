#include "LuaWindow.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "core/Locator.h"
#include "core/lua_extensions.h"
#include "video/Window.h"

namespace {
	int get_title(lua_State* L) {
		lua_pushstring(L, milk::Locator::window->title().c_str());
		return 1;
	}

	int set_title(lua_State* L) {
		if (lua_isstring(L, 1)) {
			const char* value = lua_tostring(L, 1);
			milk::Locator::window->title(value);
		}
		return 0;
	}

	int get_size(lua_State* L) {
		milk::Vector2 sz = milk::Locator::window->size();
		lua_pushinteger(L, sz.x);
		lua_pushinteger(L, sz.y);
		return 2;
	}

	int set_size(lua_State* L) {
		int w = luaL_checkinteger(L, 1);
		int h = luaL_checkinteger(L, 2);
		milk::Locator::window->size(w, h);
		return 0;
	}

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
		{ "get_title", get_title },
		{ "set_title", set_title },
		{ "get_size", get_size },
		{ "set_size", set_size },
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
