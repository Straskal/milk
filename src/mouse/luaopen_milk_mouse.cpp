#include "luaopen_milk_mouse.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Mouse.h"
#include "luamlib.h"

static int lua_mouse_is_button_down(lua_State* L)
{
	milk::mouse_buttons button = (milk::mouse_buttons)luaL_checkinteger(L, 1);
	bool down = milk::mouse_is_button_down(button);
	lua_pushboolean(L, down);
	return 1;
}

static int lua_mouse_is_button_pressed(lua_State* L)
{
	milk::mouse_buttons button = (milk::mouse_buttons)luaL_checkinteger(L, 1);
	bool pressed = milk::mouse_is_button_pressed(button);
	lua_pushboolean(L, pressed);
	return 1;
}

static int lua_mouse_is_button_released(lua_State* L)
{
	milk::mouse_buttons button = (milk::mouse_buttons)luaL_checkinteger(L, 1);
	bool released = milk::mouse_is_button_released(button);
	lua_pushboolean(L, released);
	return 1;
}


static int lua_mouse_get_position(lua_State* L)
{
	std::tuple<int, int> pos = milk::mouse_get_position();
	lua_pushinteger(L, std::get<0>(pos));
	lua_pushinteger(L, std::get<1>(pos));
	return 2;
}

static int lua_mouse_get_scroll(lua_State* L)
{
	int scroll = milk::mouse_get_scroll();
	lua_pushinteger(L, scroll);
	return 1;
}

static const luaL_Reg lua_mouse_funcs[] = {
	{ "is_button_down", lua_mouse_is_button_down },
	{ "is_button_pressed", lua_mouse_is_button_pressed },
	{ "is_button_released", lua_mouse_is_button_released },
	{ "get_position", lua_mouse_get_position },
	{ "get_scroll", lua_mouse_get_scroll },
	{ nullptr, nullptr }
};

static const milk::luaM_Enum lua_buttons_enum[] = {
	{ "LEFT", (int)milk::mouse_buttons::LEFT },
	{ "MIDDLE", (int)milk::mouse_buttons::MIDDLE },
	{ "RIGHT", (int)milk::mouse_buttons::RIGHT }
};

int luaopen_milk_mouse(lua_State* L)
{
	luaL_newlib(L, lua_mouse_funcs);
	luaM_setenumfield(L, -1, "buttons", lua_buttons_enum, sizeof(lua_buttons_enum));
	return 1;
}