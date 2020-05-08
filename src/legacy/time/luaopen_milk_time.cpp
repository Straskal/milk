#include "luaopen_milk_time.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "time.h"

static int lua_time_get_total(lua_State* L)
{
	double total = milk::time_get_total();
	lua_pushnumber(L, total);
	return 1;
}

static int lua_time_delay(lua_State* L)
{
	double seconds = (double)luaL_checknumber(L, 1);
	milk::time_delay(seconds);
	return 0;
}

static const luaL_Reg lua_time_funcs[] = {
	{ "get_total", lua_time_get_total },
	{ "delay", lua_time_delay },
	{ nullptr, nullptr }
};

int luaopen_milk_time(lua_State* L)
{
	luaL_newlib(L, lua_time_funcs);
	return 1;
}