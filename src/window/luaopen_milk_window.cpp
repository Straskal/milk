#include "luaopen_milk_window.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "window.h"

static int lua_window_show(lua_State* L)
{
	(void)(L);
	milk::window_show();
	return 0;
}

static int lua_window_get_title(lua_State* L)
{
	const char* t = milk::window_get_title();
	lua_pushstring(L, t);
	return 1;
}

static int lua_window_set_title(lua_State* L)
{
	const char* title = (const char*)luaL_checkstring(L, 1);
	milk::window_set_title(title);
	return 0;
}

static int lua_window_set_icon(lua_State* L)
{
	const char* path = (const char*)luaL_checkstring(L, 1);
	milk::window_set_icon(path);
	return 0;
}

static int lua_window_get_size(lua_State* L)
{
	std::tuple<int, int> sz = milk::window_get_size();
	lua_pushinteger(L, std::get<0>(sz));
	lua_pushinteger(L, std::get<1>(sz));
	return 2;
}

static int lua_window_set_size(lua_State* L)
{
	int w = (int)luaL_checkinteger(L, 1);
	int h = (int)luaL_checkinteger(L, 2);
	milk::window_set_size(w, h);
	return 0;
}

static int lua_window_set_fullscreen(lua_State* L)
{
	if (lua_isboolean(L, 1)) {
		bool fs = lua_toboolean(L, 1);
		milk::window_set_is_fullscreen(fs);
	}
	return 0;
}

static int lua_window_is_fullscreen(lua_State* L)
{
	bool fs = milk::window_get_is_fullscreen();
	lua_pushboolean(L, fs);
	return 1;
}

static int lua_window_close(lua_State* L)
{
	(void)(L);
	milk::window_close();
	return 0;
}

static int lua_window_should_close(lua_State* L)
{
	bool shouldClose = milk::window_should_close();
	lua_pushboolean(L, shouldClose);
	return 1;
}

static const luaL_Reg window_funcs[] = {
	{ "show", lua_window_show },
	{ "get_title", lua_window_get_title },
	{ "set_title", lua_window_set_title },
	{ "set_icon", lua_window_set_icon },
	{ "get_size", lua_window_get_size },
	{ "set_size", lua_window_set_size },
	{ "set_fullscreen", lua_window_set_fullscreen },
	{ "is_fullscreen", lua_window_is_fullscreen },
	{ "close", lua_window_close },
	{ "should_close", lua_window_should_close },
	{ nullptr, nullptr }
};

int luaopen_milk_window(lua_State* L)
{
	luaL_newlib(L, window_funcs);
	return 1;
}
