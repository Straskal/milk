#include "milk_api.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "milk.h"
#include <stdio.h>

static int l_clrs(lua_State *L)
{
	lua_getglobal(L, "__mk");
	Milk *mk = (Milk *)lua_touserdata(L, -1);
	int col = lua_tointeger(L, 1);
	milk_clear(&mk->video, col);
	return 0;
}

static int l_pset(lua_State *L)
{
	lua_getglobal(L, "__mk");
	Milk *mk = (Milk *)lua_touserdata(L, -1);
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int hex = lua_tointeger(L, 3);
	milk_pixel_set(&mk->video, hex, x, y);
	return 0;
}

static int l_rectfill(lua_State *L)
{
	lua_getglobal(L, "__mk");
	Milk *mk = (Milk *)lua_touserdata(L, -1);
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	int hex = lua_tointeger(L, 5);
	milk_rectfill(&mk->video, hex, x, y, w, h);
	return 0;
}

static int l_rect(lua_State *L)
{
	lua_getglobal(L, "__mk");
	Milk *mk = (Milk *)lua_touserdata(L, -1);
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	int hex = lua_tointeger(L, 5);
	milk_rect(&mk->video, hex, x, y, w, h);
	return 0;
}

static int l_spr(lua_State *L)
{
	lua_getglobal(L, "__mk");
	Milk *mk = (Milk *)lua_touserdata(L, -1);
	int idx = lua_tointeger(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	milk_spr(&mk->video, idx, x, y);
	return 0;
}

static void __push_api_func(lua_State *L, const char *name, int(*api_func)(lua_State *))
{
	lua_pushcfunction(L, api_func);
	lua_setglobal(L, name);
}

static void __push_api(lua_State *L)
{
	__push_api_func(L, "clrs", l_clrs);
	__push_api_func(L, "pset", l_pset);
	__push_api_func(L, "rectfill", l_rectfill);
	__push_api_func(L, "rect", l_rect);
	__push_api_func(L, "spr", l_spr);
}

void milk_load_code(Milk *milk)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_dofile(L, "main.lua"))
	{
		printf("%s\n", lua_tostring(L, -1));
	}
	lua_pushlightuserdata(L, (void *)milk);
	lua_setglobal(L, "__mk");
	__push_api(L);
	milk->code.state = (void *)L;
}

void milk_invoke_update(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_update");
	lua_pcall(L, 0, 0, 0);
}

void milk_invoke_draw(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_draw");
	lua_pcall(L, 0, 0, 0);
}
