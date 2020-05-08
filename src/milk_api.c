#include "milk_api.h"

#include <stdio.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "milk.h"

static int l_clrs(lua_State* L) 
{
	lua_getglobal(L, "__mk");
	MilkMachine* mk = (MilkMachine*)lua_touserdata(L, -1);
	int col = lua_tointeger(L, 1); 
	MilkClear(&mk->memory.vram, col);
	return 0;
}

static int l_pset (lua_State* L)
{
	lua_getglobal(L, "__mk");
	MilkMachine* mk = (MilkMachine*)lua_touserdata(L, -1);
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int idx = lua_tointeger(L, 3);
	ColorRGB color = mk->memory.vram.palette[idx];
	MilkDrawPixel(&mk->memory.vram, color, x, y);
	return 0;
}

static void PushAPIFunction(lua_State* L, const char* name, int(*api_func)(lua_State*))
{
	lua_pushcfunction(L, api_func);
	lua_setglobal(L, name);
}

static void PushAPI(lua_State* L) 
{
	PushAPIFunction(L, "clrs", l_clrs);
	PushAPIFunction(L, "pset", l_pset);
}

void MilkLoadCode(MilkMachine* milk)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_dofile(L, "main.lua"))
	{
		printf("%s\n", lua_tostring(L, -1));
	}
	lua_pushlightuserdata(L, (void*)milk);
	lua_setglobal(L, "__mk");
	PushAPI(L);
	milk->code.state = (void*)L;
}

void MilkInvokeCodeUpdate(Code* code)
{
	lua_State* L = (lua_State*)code->state;
	lua_getglobal(L, "mk_update");
	lua_pcall(L, 0, 0, 0);
}

void MilkInvokeCodeDraw(Code* code)
{
	lua_State* L = (lua_State*)code->state;
	lua_getglobal(L, "mk_draw");
	lua_pcall(L, 0, 0, 0);
}
