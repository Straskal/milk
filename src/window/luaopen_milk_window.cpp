#include "luaopen_milk_window.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Window.h"
#include "Locator.h"

static int window_show(lua_State* L)
{
	(void)(L);
	milk::State::window->show();
	return 0;
}

static int window_get_title(lua_State* L)
{
	const char* t = milk::State::window->title();
	lua_pushstring(L, t);
	return 1;
}

static int window_set_title(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* val = lua_tostring(L, 1);
		milk::State::window->title(val);
	}
	return 0;
}

static int window_set_icon(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* iconfilepath = lua_tostring(L, 1);
		milk::State::window->icon(iconfilepath);
	}
	return 0;
}

static int window_get_size(lua_State* L)
{
	std::tuple<int, int> sz = milk::State::window->size();
	lua_pushinteger(L, std::get<0>(sz));
	lua_pushinteger(L, std::get<1>(sz));
	return 2;
}

static int window_set_size(lua_State* L)
{
	int w = (int)luaL_checkinteger(L, 1);
	int h = (int)luaL_checkinteger(L, 2);
	milk::State::window->size(w, h);
	return 0;
}

static int window_set_fullscreen(lua_State* L)
{
	if (lua_isboolean(L, 1)) {
		bool fs = lua_toboolean(L, 1);
		milk::State::window->fullscreen(fs);
	}
	return 0;
}

static int window_is_fullscreen(lua_State* L)
{
	bool fs = milk::State::window->fullscreen();
	lua_pushboolean(L, fs);
	return 1;
}

static int window_close(lua_State* L)
{
    (void)(L);
	milk::State::window->close();
	return 0;
}

static int window_should_close(lua_State* L)
{
	bool shouldClose = milk::State::window->shouldClose();
	lua_pushboolean(L, shouldClose);
	return 1;
}

static const luaL_Reg window_funcs[] = {
	{ "show", window_show },
	{ "get_title", window_get_title },
	{ "set_title", window_set_title },
	{ "set_icon", window_set_icon },
	{ "get_size", window_get_size },
	{ "set_size", window_set_size },
	{ "set_fullscreen", window_set_fullscreen },
	{ "is_fullscreen", window_is_fullscreen },
	{ "close", window_close },
	{ "should_close", window_should_close },
	{ nullptr, nullptr }
};

int luaopen_milk_window(lua_State* L)
{
	luaL_newlib(L, window_funcs);
	return 1;
}
