#include "LuaWindow.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "core/Locator.h"
#include "core/lua_extensions.h"
#include "video/Window.h"

static int get_title(lua_State* L) {
	lua_pushstring(L, milk::Locator::window->title().c_str());
	return 1;
}

static int set_title(lua_State* L) {
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::Locator::window->title(value);
	}
	return 0;
}

static int get_size(lua_State* L) {
	std::tuple<int, int> dim = milk::Locator::window->dimensions();
	lua_pushinteger(L, std::get<0>(dim));
	lua_pushinteger(L, std::get<1>(dim));
	return 2;
}

static int set_size(lua_State* L) {
	int w = luaL_checkinteger(L, 1);
	int h = luaL_checkinteger(L, 2);
	milk::Locator::window->size(w, h);
	return 0;
}

static int set_fullscreen(lua_State* L) {
	if (lua_isboolean(L, 1)) {
		bool toggle = lua_toboolean(L, 1);
		milk::Locator::window->fullscreen(toggle);
	}
	return 0;
}

static int is_fullscreen(lua_State* L) {
	bool fs = milk::Locator::window->fullscreen();
	lua_pushboolean(L, fs);
	return 1;
}

static int close(lua_State* L) {
	milk::Locator::window->close();
	return 0;
}

static const luaL_Reg funcs[] = {
	{ "get_title", get_title },
	{ "set_title", set_title },
	{ "get_size", get_size },
	{ "set_size", set_size },
	{ "set_fullscreen", set_fullscreen },
	{ "is_fullscreen", is_fullscreen },
	{ "close", close },
	{ NULL, NULL }
};

void milk::LuaWindow::pushWindowTable(lua_State* L) {
	luaL_newlib(L, funcs);
}
