/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software andassociated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, andto permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice andthis permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "milkapi.h"
#include "milk.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>
#include <stdio.h>

static Milk *_globalMilk;
static void _pushApi(lua_State *L);

void milkLoadCode(Milk *milk)
{
	if (milk->code.state == NULL)
	{
		_globalMilk = milk;

		lua_State *L = luaL_newstate();
		luaL_openlibs(L);
		milk->code.state = (void *)L;
		_pushApi(L);

		if (luaL_dofile(L, "main.lua"))
		{
			milkLog(milk, lua_tostring(L, -1), ERROR);
			return;
		}

		lua_getglobal(L, "_init"); /* Invoke _init callback. */
		lua_call(L, 0, 0);
	}
}

void milkUnloadCode(Milk *milk)
{
	if (milk->code.state != NULL)
	{
		lua_close((lua_State *)milk->code.state);
		milk->code.state = NULL;
		_globalMilk = NULL;
	}
}

void milkInvokeUpdate(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_update");

	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		milkLog(_globalMilk, lua_tostring(L, -1), ERROR);
		lua_pop(L, -1);
	}
}

void milkInvokeDraw(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_draw");

	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		milkLog(_globalMilk, lua_tostring(L, -1), ERROR);
		lua_pop(L, -1);
	}
}

static int l_btn(lua_State *L)
{
	lua_pushboolean(L,
		milkButton(&_globalMilk->input, (uint8_t)(1 << lua_tointeger(L, 1)))
	);
	return 1;
}

static int l_btnp(lua_State *L)
{
	lua_pushboolean(L,
		milkButtonPressed(&_globalMilk->input, (uint8_t)(1 << lua_tointeger(L, 1)))
	);
	return 1;
}

static int l_vol(lua_State *L)
{
	milkVolume(&_globalMilk->audio, (uint8_t)lua_tointeger(L, 1));
	return 0;
}

static int l_snd(lua_State *L)
{
	milkSound(&_globalMilk->audio,
		(int)lua_tointeger(L, 1),
		(uint8_t)luaL_optinteger(L, 2, 128),
		(uint8_t)luaL_optinteger(L, 3, 0)
	);
	return 0;
}

static int l_clip(lua_State *L)
{
	milkClipRect(&_globalMilk->video,
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4)
	);
	return 0;
}

static int l_clrs(lua_State *L)
{
	milkClear(&_globalMilk->video, (Color32)luaL_optinteger(L, 1, 0x000000));
	return 0;
}

static int l_pset(lua_State *L)
{
	milkPixelSet(&_globalMilk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(Color32)lua_tointeger(L, 3)
	);
	return 0;
}

static int l_rectfill(lua_State *L)
{
	milkRectFill(&_globalMilk->video,
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
	milkRect(&_globalMilk->video,
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
	milkSprite(&_globalMilk->video,
		(int)lua_tointeger(L, 1),
		(int)floor(lua_tonumber(L, 2)),
		(int)floor(lua_tonumber(L, 3)),
		(int)luaL_optinteger(L, 4, 1),
		(int)luaL_optinteger(L, 5, 1),
		(float)luaL_optnumber(L, 6, 1.0),
		(int)luaL_optinteger(L, 7, 0)
	);
	return 0;
}

static int l_sprfont(lua_State *L)
{
	milkSpriteFont(
		&_globalMilk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		lua_tostring(L, 3),
		(float)luaL_optnumber(L, 4, 1.0),
		(Color32)luaL_optinteger(L, 5, 0xffffff)
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
	_pushApiFunction(L, "btn", l_btn);
	_pushApiFunction(L, "btnp", l_btnp);
	_pushApiFunction(L, "vol", l_vol);
	_pushApiFunction(L, "snd", l_snd);
	_pushApiFunction(L, "clip", l_clip);
	_pushApiFunction(L, "clrs", l_clrs);
	_pushApiFunction(L, "pset", l_pset);
	_pushApiFunction(L, "rectfill", l_rectfill);
	_pushApiFunction(L, "rect", l_rect);
	_pushApiFunction(L, "spr", l_spr);
	_pushApiFunction(L, "sprfont", l_sprfont);
}
