#include "milk_api.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "milk.h"
#include <math.h>
#include <stdio.h>

static Milk *_getGlobalMilk(lua_State *L)
{
	lua_getglobal(L, "__milk");
	return (Milk *)lua_touserdata(L, -1);
}

static int l_clrs(lua_State *L)
{
	Milk *milk = _getGlobalMilk(L);
	milkClear(&milk->video, (Color32)lua_tointeger(L, 1));
	return 0;
}

static int l_pset(lua_State *L)
{
	Milk *milk = _getGlobalMilk(L);
	milkPixelSet(&milk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(Color32)lua_tointeger(L, 3)
	);
	return 0;
}

static int l_rectfill(lua_State *L)
{
	Milk *milk = _getGlobalMilk(L);
	milkRectFill(&milk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(Color32)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_rect(lua_State *L)
{
	Milk *milk = _getGlobalMilk(L);
	milkRect(&milk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(Color32)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_spr(lua_State *L)
{
	Milk *milk = _getGlobalMilk(L);
	milkSprite(&milk->video,
		(int)lua_tointeger(L, 1),
		(int)floor(lua_tonumber(L, 2)),
		(int)floor(lua_tonumber(L, 3))
	);
	return 0;
}

static int l_btn(lua_State *L)
{
	Milk *milk = _getGlobalMilk(L);
	lua_pushboolean(L,
		milkButton(&milk->input, (uint8_t)(1 << lua_tointeger(L, 1)))
	);
	return 1;
}

static int l_sprfont(lua_State *L)
{
	Milk *milk = _getGlobalMilk(L);
	milkSpriteFont(
		&milk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		lua_tostring(L, 3)
	);
	return 1;
}

static void _pushApiFunction(lua_State *L, const char *name, int(*api_func)(lua_State *))
{
	lua_pushcfunction(L, api_func);
	lua_setglobal(L, name);
}

static void _pushApi(lua_State *L)
{
	_pushApiFunction(L, "clrs", l_clrs);
	_pushApiFunction(L, "pset", l_pset);
	_pushApiFunction(L, "rectfill", l_rectfill);
	_pushApiFunction(L, "rect", l_rect);
	_pushApiFunction(L, "spr", l_spr);
	_pushApiFunction(L, "btn", l_btn);
	_pushApiFunction(L, "sprfont", l_sprfont);
}

void milkLoadScripts(Milk *milk)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_dofile(L, "main.lua"))
	{
		printf("%s\n", lua_tostring(L, -1));
	}
	lua_pushlightuserdata(L, (void *)milk);
	lua_setglobal(L, "__milk");
	_pushApi(L);
	milk->code.state = (void *)L;
}

void milkInvokeUpdate(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_update");
	lua_call(L, 0, 0);
}

void milkInvokeDraw(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_draw");
	lua_call(L, 0, 0);
}
