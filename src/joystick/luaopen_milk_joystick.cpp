#include "luaopen_milk_joystick.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "joystick.h"
#include "luamlib.h"

static int lua_joystick_get_x(lua_State* L)
{
	int x = milk::joystick_get_x();
	lua_pushinteger(L, x);
	return 1;
}

static int lua_joystick_get_y(lua_State* L)
{
	int y = milk::joystick_get_y();
	lua_pushinteger(L, y);
	return 1;
}

static const luaL_Reg lua_joystick_funcs[] = {
	{ "get_x", lua_joystick_get_x },
    { "get_y", lua_joystick_get_y },
	{ nullptr, nullptr }
};

int luaopen_milk_joystick(lua_State* L)
{
	luaL_newlib(L, lua_joystick_funcs);
	return 1;
}