#include "luaopen_milk_window.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Window.h"
#include "core/Locator.h"

static int window_get_title(lua_State* L)
{
	const char* t = milk::Locator::window->title();
	lua_pushstring(L, t);
	return 1;
}

static int window_set_title(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* val = lua_tostring(L, 1);
		milk::Locator::window->title(val);
	}
	return 0;
}

static int window_set_icon(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* iconfilepath = lua_tostring(L, 1);
		milk::Locator::window->icon(iconfilepath);
	}
	return 0;
}

static int window_get_size(lua_State* L)
{
	std::tuple<int, int> sz = milk::Locator::window->size();
	lua_pushinteger(L, std::get<0>(sz));
	lua_pushinteger(L, std::get<1>(sz));
	return 2;
}

static int window_set_size(lua_State* L)
{
	int w = (int)luaL_checkinteger(L, 1);
	int h = (int)luaL_checkinteger(L, 2);
	milk::Locator::window->size(w, h);
	return 0;
}

static int window_set_fullscreen(lua_State* L)
{
	if (lua_isboolean(L, 1)) {
		bool fs = lua_toboolean(L, 1);
		milk::Locator::window->fullscreen(fs);
	}
	return 0;
}

static int window_is_fullscreen(lua_State* L)
{
	bool fs = milk::Locator::window->fullscreen();
	lua_pushboolean(L, fs);
	return 1;
}

static int window_close(lua_State* L)
{
	milk::Locator::window->close();
	return 0;
}

static const luaL_Reg window_funcs[] = {
	{ "get_title", window_get_title },
	{ "set_title", window_set_title },
	{ "set_icon", window_set_icon },
	{ "get_size", window_get_size },
	{ "set_size", window_set_size },
	{ "set_fullscreen", window_set_fullscreen },
	{ "is_fullscreen", window_is_fullscreen },
	{ "close", window_close },
	{ NULL, NULL }
};

int milk::luaopen_milk_window(lua_State* L)
{
	luaL_newlib(L, window_funcs);
	return 1;
}
