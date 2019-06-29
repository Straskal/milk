#include "luaopen_milk_time.h"

#include <algorithm>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Time.h"
#include "core/Locator.h"

static int time_get_total(lua_State* L)
{
	double total = milk::Locator::time->total;
	lua_pushnumber(L, total);
	return 1;
}

static int time_get_scale(lua_State* L)
{
	double scale = milk::Locator::time->scale;
	lua_pushnumber(L, scale);
	return 1;
}

static int time_set_scale(lua_State* L)
{
	float scale = (float)luaL_checknumber(L, 1);
	milk::Locator::time->scale = std::min(std::max(scale, 0.f), 1.f);
	return 1;
}

static int time_get_delta(lua_State* L)
{
	milk::Time* time = milk::Locator::time;
	double scaledDelta = time->delta * time->scale;
	lua_pushnumber(L, scaledDelta);
	return 1;
}

static const luaL_Reg time_funcs[] = {
	{ "get_total", time_get_total },
	{ "get_scale", time_get_scale },
	{ "set_scale", time_set_scale },
	{ "get_delta", time_get_delta },
	{ NULL, NULL }
};

int milk::luaopen_milk_time(lua_State* L)
{
	luaL_newlib(L, time_funcs);
	return 1;
}
