/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "api.h"
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
			LOG_ERROR(milk, lua_tostring(L, -1));
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

void milkInvokeInit(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_init"); /* Invoke _init callback. */
	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		LOG_ERROR(_globalMilk, lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}

void milkInvokeUpdate(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_update");
	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		LOG_ERROR(_globalMilk, lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}

void milkInvokeDraw(Code *code)
{
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_draw");
	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		LOG_ERROR(_globalMilk, lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}

static int l_btn(lua_State *L)
{
	lua_pushboolean(L,
		isButtonDown(&_globalMilk->input, (ButtonState)(1 << lua_tointeger(L, 1)))
	);
	return 1;
}

static int l_btnp(lua_State *L)
{
	lua_pushboolean(L,
		isButtonPressed(&_globalMilk->input, (ButtonState)(1 << lua_tointeger(L, 1)))
	);
	return 1;
}

static int l_loadspr(lua_State *L)
{
	loadSpriteSheet(&_globalMilk->video,
	                lua_tostring(L, 1)
	);
	return 0;
}

static int l_loadfont(lua_State *L)
{
	loadFont(&_globalMilk->video,
		lua_tostring(L, 1)
	);
	return 0;
}

static int l_clip(lua_State *L)
{
	setClippingRect(&_globalMilk->video,
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4)
	);
	return 0;
}

static int l_clrs(lua_State *L)
{
	clearFramebuffer(&_globalMilk->video, (Color32)luaL_optinteger(L, 1, 0x000000));
	return 0;
}

static int l_pset(lua_State *L)
{
	blitPixel(&_globalMilk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(Color32)lua_tointeger(L, 3)
	);
	return 0;
}

static int l_rect(lua_State *L)
{
	blitRectangle(&_globalMilk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(Color32)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_rectfill(lua_State *L)
{
	blitFilledRectangle(&_globalMilk->video,
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
	blitSprite(&_globalMilk->video,
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
	blitSpriteFont(&_globalMilk->video, _globalMilk->video.font,
	               (int) floor(lua_tonumber(L, 1)),
	               (int) floor(lua_tonumber(L, 2)),
	               lua_tostring(L, 3),
	               (float) luaL_optnumber(L, 4, 1.0),
	               (Color32) luaL_optinteger(L, 5, 0xffffff)
	);
	return 1;
}

static int l_loadsnd(lua_State *L)
{
	loadSound(&_globalMilk->audio,
		(int)lua_tointeger(L, 1),
		lua_tostring(L, 2)
	);
	return 0;
}

static int l_freesnd(lua_State *L)
{
	unloadSound(&_globalMilk->audio,
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_play(lua_State *L)
{
	playSound(&_globalMilk->audio,
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3)
	);
	return 0;
}

static int l_pause(lua_State *L)
{
	pauseSound(&_globalMilk->audio,
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_resume(lua_State *L)
{
	resumeSound(&_globalMilk->audio,
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_stop(lua_State *L)
{
	stopSound(&_globalMilk->audio,
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_sndslot(lua_State *L)
{
	lua_pushinteger(L,
		getSampleState(&_globalMilk->audio, (int)lua_tointeger(L, 1))
	);
	return 1;
}

static int l_vol(lua_State *L)
{
	setMasterVolume(&_globalMilk->audio,
		(uint8_t)lua_tointeger(L, 1)
	);
	return 0;
}

static void _pushApiFunction(lua_State *L, const char *name, int(*api_func)(lua_State *))
{
	lua_pushcfunction(L, api_func);
	lua_setglobal(L, name);
}

static void _pushApi(lua_State *L)
{
	_pushApiFunction(L, "btn",		l_btn);
	_pushApiFunction(L, "btnp",		l_btnp);
	_pushApiFunction(L, "loadspr",	l_loadspr);
	_pushApiFunction(L, "loadfont", l_loadfont);
	_pushApiFunction(L, "clip",		l_clip);
	_pushApiFunction(L, "clrs",		l_clrs);
	_pushApiFunction(L, "pset",		l_pset);
	_pushApiFunction(L, "rect",		l_rect);
	_pushApiFunction(L, "rectfill", l_rectfill);
	_pushApiFunction(L, "spr",		l_spr);
	_pushApiFunction(L, "sprfont",	l_sprfont);
	_pushApiFunction(L, "loadsnd",	l_loadsnd);
	_pushApiFunction(L, "freesnd",	l_freesnd);
	_pushApiFunction(L, "play",		l_play);
	_pushApiFunction(L, "stop",		l_stop);
	_pushApiFunction(L, "pause",	l_pause);
	_pushApiFunction(L, "resume",	l_resume);
	_pushApiFunction(L, "sndslot",	l_sndslot);
	_pushApiFunction(L, "vol",		l_vol);
}
