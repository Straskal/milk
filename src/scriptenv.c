#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>
#include <stdint.h>

#include "bitmap.h"
#include "common.h"
#include "logs.h"
#include "milk.h"
#include "platform.h"
#include "scriptenv.h"

static const char ModuleRegistryKey = 'k';

#define BITMAP_META "bitmap"
#define WAVE_META "wave"
#define WAVESTREAM_META "wavestream"

typedef struct
{
	void *handle;
} LuaObject;

static Modules *__getModules(lua_State *L)
{
	lua_pushlightuserdata(L, (void *)&ModuleRegistryKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	Modules *modules = lua_touserdata(L, -1);
	lua_pop(L, 1);
	return modules;
}

#define input_addr(L) (&__getModules(L)->input)
#define audio_addr(L) (&__getModules(L)->audio)
#define video_addr(L) (&__getModules(L)->video)

static int l_btn(lua_State *L)
{
	lua_pushboolean(L,
		isButtonDown(
			input_addr(L),
			(ButtonState)(lua_tointeger(L, 1))
		)
	);
	return 1;
}

static int l_btnp(lua_State *L)
{
	lua_pushboolean(L,
		isButtonPressed(
			input_addr(L),
			(ButtonState)(lua_tointeger(L, 1))
		)
	);
	return 1;
}

static int l_key(lua_State *L)
{
	lua_pushboolean(L,
		isKeyDown(
			input_addr(L),
			(lua_tointeger(L, 1))
		)
	);
	return 1;
}

static int l_keyp(lua_State *L)
{
	lua_pushboolean(L,
		isKeyPressed(
			input_addr(L),
			(lua_tointeger(L, 1))
		)
	);
	return 1;
}

static int l_mouse(lua_State *L)
{
	Input *input = input_addr(L);
	Mouse *mouse = &input->mouse;

	lua_pushnumber(L, mouse->x);
	lua_pushnumber(L, mouse->y);
	lua_pushnumber(L, mouse->scroll);

	return 3;
}

static int l_mousebtn(lua_State *L)
{
	lua_pushboolean(L,
		isMouseDown(
			input_addr(L),
			lua_tointeger(L, 1)
		)
	);
	return 1;
}

static int l_mousebtnp(lua_State *L)
{
	lua_pushboolean(L,
		isMousePressed(
			input_addr(L),
			lua_tointeger(L, 1)
		)
	);
	return 1;
}

static int l_bitmap(lua_State *L)
{
	Bitmap *bmp = loadBitmap(lua_tostring(L, 1));
	if (!bmp)
	{
		lua_pushstring(L, getError());
		return lua_error(L);
	}
	else
	{
		LuaObject *luaObj = lua_newuserdata(L, sizeof(LuaObject));
		luaObj->handle = bmp;
		luaL_setmetatable(L, BITMAP_META);
	}
	return 1;
}

static int l_bitmap_gc(lua_State *L)
{
	LuaObject *luaObj = lua_touserdata(L, 1);
	Bitmap *bmp = luaObj->handle;
	freeBitmap(bmp);
	return 0;
}

static int l_clip(lua_State *L)
{
	setClip(
		video_addr(L),
		lua_tointeger(L, 1),
		lua_tointeger(L, 2),
		lua_tointeger(L, 3),
		lua_tointeger(L, 4)
	);
	return 0;
}

static int l_clrs(lua_State *L)
{
	clearFramebuffer(
		video_addr(L),
		(uint32_t)luaL_optinteger(L, 1, 0x000000)
	);
	return 0;
}

static int l_pset(lua_State *L)
{
	drawPixel(
		video_addr(L),
		(int)FLOOR(lua_tonumber(L, 1)),
		(int)FLOOR(lua_tonumber(L, 2)),
		(uint32_t)lua_tointeger(L, 3)
	);
	return 0;
}

static int l_line(lua_State *L)
{
	drawLine(
		video_addr(L),
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_rect(lua_State *L)
{
	drawRect(
		video_addr(L),
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_rectfill(lua_State *L)
{
	drawFilledRect(
		video_addr(L),
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_sprite(lua_State *L)
{
	LuaObject *luaObj = luaL_checkudata(L, 1, BITMAP_META);
	Bitmap *bmp = luaObj->handle;

	drawSprite(
		video_addr(L), bmp,
		(int)lua_tointeger(L, 2),
		(int)floor(lua_tonumber(L, 3)),
		(int)floor(lua_tonumber(L, 4)),
		(int)luaL_optinteger(L, 5, 1),
		(int)luaL_optinteger(L, 6, 1),
		(float)luaL_optnumber(L, 7, 1.0),
		(uint8_t)luaL_optinteger(L, 8, 0),
		(uint32_t)luaL_optinteger(L, 9, 0)
	);
	return 0;
}

static int l_tiles(lua_State *L)
{
	Video *video = video_addr(L);
	LuaObject *luaObj = luaL_checkudata(L, 1, BITMAP_META);
	Bitmap *bmp = luaObj->handle;

	int x = FLOOR(lua_tonumber(L, 3));
	int y = FLOOR(lua_tonumber(L, 4));
	int w = lua_tointeger(L, 5);
	int h = lua_tointeger(L, 6);
	int wPix = w * SPRITE_SIZE;
	int hPix = h * SPRITE_SIZE;

	int pitch = lua_tointeger(L, 7);
	lua_len(L, 2);
	int len = lua_tointeger(L, -1);
	lua_pop(L, 1);

	int xCurrent = x;
	int i = 1;

	Rect clip = video->clipRect;

	while (len--)
	{
		lua_rawgeti(L, 2, i);
		int sprIndex = lua_tointeger(L, -1);
		lua_pop(L, 1);

		int right = xCurrent + wPix;
		int bottom = y + hPix;

		if (sprIndex > -1 && (right > clip.left || xCurrent < clip.right) && (bottom > clip.top || y < clip.top))
			drawSprite(video, bmp, sprIndex, xCurrent, y, w, h, 1, 0, 0);

		xCurrent += w * SPRITE_SIZE;

		if (i++ % pitch == 0)
		{
			xCurrent = x;
			y += h * SPRITE_SIZE;
		}
	}
	return 0;
}

static int l_font(lua_State *L)
{
	Bitmap *bmp = NULL;
	if (!lua_isnil(L, 1))
	{
		LuaObject *luaObj = luaL_checkudata(L, 1, BITMAP_META);
		if (luaObj)
			bmp = luaObj->handle;
	}

	drawFont(
		video_addr(L), bmp,
		(int)floor(lua_tonumber(L, 2)),
		(int)floor(lua_tonumber(L, 3)),
		lua_tostring(L, 4),
		(int)floor(luaL_optinteger(L, 5, 1)),
		(uint32_t)floor(luaL_optinteger(L, 6, 0xffffffff))
	);
	return 1;
}

static int l_fontwrap(lua_State *L)
{
	Bitmap *bmp = NULL;
	if (!lua_isnil(L, 1))
	{
		LuaObject *luaObj = luaL_checkudata(L, 1, BITMAP_META);
		if (luaObj)
			bmp = luaObj->handle;
	}

	drawWrappedFont
		(video_addr(L), bmp,
		(int)floor(lua_tonumber(L, 2)),
		(int)floor(lua_tonumber(L, 3)),
		(int)floor(lua_tonumber(L, 4)),
		lua_tostring(L, 5),
		(int)floor(luaL_optinteger(L, 6, 1)),
		(uint32_t)floor(luaL_optinteger(L, 7, 0xffffffff))
	);
	return 1;
}

static int l_wave(lua_State *L)
{
	Wave *wave = loadWave(lua_tostring(L, 1));
	if (!wave)
		lua_pushnil(L);
	else
	{
		LuaObject *luaObj = lua_newuserdata(L, sizeof(LuaObject));
		luaObj->handle = wave;
		luaL_setmetatable(L, WAVE_META);
	}
	return 1;
}

static int l_wave_gc(lua_State *L)
{
	LuaObject *luaObj = lua_touserdata(L, 1);
	Wave *wave = luaObj->handle;
	stopInstances(audio_addr(L), wave);
	freeWave(wave);
	return 0;
}

static int l_wavestream(lua_State *L)
{
	const char *filePath = lua_tostring(L, 1);
	WaveStream *waveStream = openWaveStream(filePath);
	if (!waveStream)
		lua_pushnil(L);
	else
	{
		LuaObject *luaObj = lua_newuserdata(L, sizeof(LuaObject));
		luaObj->handle = waveStream;
		luaL_setmetatable(L, WAVESTREAM_META);
	}
	return 1;
}

static int l_wavestream_gc(lua_State *L)
{
	LuaObject *luaObj = lua_touserdata(L, 1);
	WaveStream *waveStream = luaObj->handle;
	if (__getModules(L)->audio.streamSlot.data == waveStream)
		stopStream(audio_addr(L));
	closeWaveStream(waveStream);
	return 0;
}

static int l_play(lua_State *L)
{
	LuaObject *luaObj = lua_touserdata(L, 1);
	Wave *wave = luaObj->handle;
	playSound(audio_addr(L), wave, (int)lua_tointeger(L, 2), (int)lua_tointeger(L, 3));
	return 0;
}

static int l_pause(lua_State *L)
{
	pauseSound(
		audio_addr(L),
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_resume(lua_State *L)
{
	resumeSound(
		audio_addr(L),
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_stop(lua_State *L)
{
	stopSound(
		audio_addr(L),
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_sndslot(lua_State *L)
{
	lua_pushinteger(L,
		getSoundState(
			audio_addr(L),
			(int)lua_tointeger(L, 1)
		)
	);
	return 1;
}

static int l_playstream(lua_State *L)
{
	LuaObject *luaObj = lua_touserdata(L, 1);
	WaveStream *waveStream = luaObj->handle;

	bool loop = false;
	if (lua_isboolean(L, 3))
		loop = lua_toboolean(L, 3);

	playStream(
		audio_addr(L),
		waveStream,
		(int)lua_tointeger(L, 2),
		loop
	);
	return 0;
}

static int l_stopstream(lua_State *L)
{
	UNUSED(L);
	stopStream(audio_addr(L));
	return 0;
}

static int l_pausestream(lua_State *L)
{
	UNUSED(L);
	pauseStream(audio_addr(L));
	return 0;
}

static int l_resumestream(lua_State *L)
{
	UNUSED(L);
	resumeStream(audio_addr(L));
	return 0;
}

static int l_vol(lua_State *L)
{
	setMasterVolume(audio_addr(L), lua_tointeger(L, 1));
	return 0;
}

static int l_exit(lua_State *L)
{
	UNUSED(L);
	platform_close();
	return 0;
}

static void __registerModules(lua_State *L, Modules *modules)
{
	lua_pushlightuserdata(L, (void *)&ModuleRegistryKey);
	lua_pushlightuserdata(L, (void *)modules);
	lua_settable(L, LUA_REGISTRYINDEX);
}

static void __pushApiFunction(lua_State *L, const char *name, int (*api_func)(lua_State *))
{
	lua_pushcfunction(L, api_func);
	lua_setglobal(L, name);
}

static void __registerApiFunctions(lua_State *L)
{
	__pushApiFunction(L, "key", l_key);
	__pushApiFunction(L, "keyp", l_keyp);
	__pushApiFunction(L, "btn", l_btn);
	__pushApiFunction(L, "btnp", l_btnp);
	__pushApiFunction(L, "mouse", l_mouse);
	__pushApiFunction(L, "mousebtn", l_mousebtn);
	__pushApiFunction(L, "mousebtnp", l_mousebtnp);
	__pushApiFunction(L, "bitmap", l_bitmap);
	__pushApiFunction(L, "clip", l_clip);
	__pushApiFunction(L, "clrs", l_clrs);
	__pushApiFunction(L, "pset", l_pset);
	__pushApiFunction(L, "line", l_line);
	__pushApiFunction(L, "rect", l_rect);
	__pushApiFunction(L, "rectfill", l_rectfill);
	__pushApiFunction(L, "sprite", l_sprite);
	__pushApiFunction(L, "tiles", l_tiles);
	__pushApiFunction(L, "font", l_font);
	__pushApiFunction(L, "fontwrap", l_fontwrap);
	__pushApiFunction(L, "wave", l_wave);
	__pushApiFunction(L, "stream", l_wavestream);
	__pushApiFunction(L, "play", l_play);
	__pushApiFunction(L, "pause", l_pause);
	__pushApiFunction(L, "stop", l_stop);
	__pushApiFunction(L, "resume", l_resume);
	__pushApiFunction(L, "playstream", l_playstream);
	__pushApiFunction(L, "stopstream", l_stopstream);
	__pushApiFunction(L, "pausestream", l_pausestream);
	__pushApiFunction(L, "resumestream", l_resumestream);
	__pushApiFunction(L, "sndslot", l_sndslot);
	__pushApiFunction(L, "vol", l_vol);
	__pushApiFunction(L, "exit", l_exit);
}

static void __registerMetatable(lua_State *L, const char *name, lua_CFunction gc)
{
	luaL_newmetatable(L, name);
	lua_pushcfunction(L, gc);
	lua_setfield(L, -2, "__gc");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
}

void openScriptEnv(ScriptEnv *scriptEnv, Modules *modules)
{
	lua_State *L = luaL_newstate();
	scriptEnv->state = (void *)L;
	luaL_openlibs(L);
	__registerModules(L, modules);
	__registerApiFunctions(L);
	__registerMetatable(L, BITMAP_META, l_bitmap_gc);
	__registerMetatable(L, WAVE_META, l_wave_gc);
	__registerMetatable(L, WAVESTREAM_META, l_wavestream_gc);
}

void closeScriptEnv(ScriptEnv *scriptEnv)
{
	lua_close(scriptEnv->state);
	scriptEnv->state = NULL;
}

bool loadEntryPoint(ScriptEnv *scriptEnv)
{
	lua_State *L = scriptEnv->state;
	if (luaL_dofile(L, "main.lua"))
	{
		logError(lua_tostring(L, -1));
		lua_pop(L, -1);
		return false;
	}
	return true;
}

bool invokeInit(ScriptEnv *scriptEnv)
{
	lua_State *L = scriptEnv->state;
	lua_getglobal(L, "_init");
	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		logError(lua_tostring(L, -1));
		lua_pop(L, -1);
		return false;
	}
	return true;
}

void invokeUpdate(ScriptEnv *scriptEnv)
{
	lua_State *L = scriptEnv->state;
	lua_getglobal(L, "_update");
	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		logError(lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}

void invokeDraw(ScriptEnv *scriptEnv)
{
	lua_State *L = scriptEnv->state;
	lua_getglobal(L, "_draw");
	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		logError(lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}
