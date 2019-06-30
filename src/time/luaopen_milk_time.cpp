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
	double total = milk::Locator::time->total();
	lua_pushnumber(L, total);
	return 1;
}

static int time_get_target_fps(lua_State* L) 
{
	double targetFps = milk::Locator::time->targetFps();
	lua_pushnumber(L, targetFps);
	return 1;
}

static int time_set_target_fps(lua_State* L)
{
	double targetFps = (double)luaL_checknumber(L, 1);
	milk::Locator::time->targetFps(targetFps);
	return 0;
}

static int time_get_fps(lua_State* L)
{
	double fps = milk::Locator::time->fps();
	lua_pushnumber(L, fps);
	return 1;
}

static int time_get_delta(lua_State* L)
{
	double delta = milk::Locator::time->delta();
	lua_pushnumber(L, delta);
	return 1;
}

static int time_get_scale(lua_State* L)
{
	double scale = milk::Locator::time->scale();
	lua_pushnumber(L, scale);
	return 1;
}

static int time_set_scale(lua_State* L)
{
	float scale = (float)luaL_checknumber(L, 1);
	milk::Locator::time->scale(scale);
	return 1;
}

static const luaL_Reg time_funcs[] = {
	{ "get_total", time_get_total },
	{ "get_target_fps", time_get_target_fps },
	{ "set_target_fps", time_set_target_fps },
	{ "get_fps", time_get_fps },
	{ "get_delta", time_get_delta },
	{ "get_scale", time_get_scale },
	{ "set_scale", time_set_scale },
	{ NULL, NULL }
};

int milk::luaopen_milk_time(lua_State* L)
{
	luaL_newlib(L, time_funcs);
	return 1;
}
