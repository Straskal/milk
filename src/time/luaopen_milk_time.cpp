#include "luaopen_milk_time.h"

#include <algorithm>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Time.h"
#include "State.h"

static int time_get_total(lua_State* L)
{
	double total = milk::State::time->total();
	lua_pushnumber(L, total);
	return 1;
}

static int time_delay(lua_State* L)
{
	double seconds = (double)luaL_checknumber(L, 1);
	milk::State::time->delay(seconds);
	return 0;
}

static const luaL_Reg time_funcs[] = {
	{ "get_total", time_get_total },
	{ "delay", time_delay },
	{ nullptr, nullptr }
};

int luaopen_milk_time(lua_State* L)
{
	luaL_newlib(L, time_funcs);
	return 1;
}
