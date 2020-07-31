#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>
#include <stdint.h>

#include "bitmap.h"
#include "common.h"
#include "logs.h"
#include "milk.h"

#define BITMAP_META "bitmap"
#define WAVE_META "wave"
#define WAVESTREAM_META "wavestream"

static const char MilkRegistryKey = 'k';

typedef struct {
	void *handle;
} LuaObject;

Milk *createMilk() {
	Milk *milk = malloc(sizeof(Milk));
	milk->shouldQuit = false;
	milk->code.state = NULL;
	initializeInput(&milk->input);
	initializeVideo(&milk->video);
	initializeAudio(&milk->audio);
	LOG_INIT();
	return milk;
}

void freeMilk(Milk *milk) {
	disableAudio(&milk->audio);
	disableVideo(&milk->video);
	free(milk);
}

static Milk *__getMilkFromReg(lua_State *L) {
	lua_pushlightuserdata(L, (void *)&MilkRegistryKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	Milk *milk = lua_touserdata(L, -1);
	lua_pop(L, 1);
	return milk;
}

#define input_addr(L) (&__getMilkFromReg(L)->input)
#define video_addr(L) (&__getMilkFromReg(L)->video)
#define audio_addr(L) (&__getMilkFromReg(L)->audio)

static int l_btn(lua_State *L) {
	lua_pushboolean(L, isButtonDown(input_addr(L), (ButtonState)(1 << lua_tointeger(L, 1))));
	return 1;
}

static int l_btnp(lua_State *L) {
	lua_pushboolean(L, isButtonPressed(input_addr(L), (ButtonState)(1 << lua_tointeger(L, 1))));
	return 1;
}

static int l_bitmap(lua_State *L) {
	Bitmap *bmp = loadBitmap(lua_tostring(L, 1));
	if (!bmp)
		lua_pushnil(L);
	else {
		LuaObject *luaObj = lua_newuserdata(L, sizeof(LuaObject));
		luaObj->handle = bmp;
		luaL_setmetatable(L, BITMAP_META);
	}
	return 1;
}

static int l_bitmap_gc(lua_State *L) {
	LuaObject *luaObj = lua_touserdata(L, 1);
	Bitmap *bmp = luaObj->handle;
	freeBitmap(bmp);
	return 0;
}

static int l_clip(lua_State *L) {
	setClip(video_addr(L),
		lua_tointeger(L, 1),
		lua_tointeger(L, 2),
		lua_tointeger(L, 3),
		lua_tointeger(L, 4)
	);
	return 0;
}

static int l_clrs(lua_State *L) {
	clearFramebuffer(video_addr(L), (uint32_t)luaL_optinteger(L, 1, 0x000000));
	return 0;
}

static int l_pset(lua_State *L) {
	drawPixel(video_addr(L),
		(int)FLOOR(lua_tonumber(L, 1)),
		(int)FLOOR(lua_tonumber(L, 2)),
		(uint32_t)lua_tointeger(L, 3)
	);
	return 0;
}

static int l_line(lua_State *L) {
	drawLine(video_addr(L),
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_rect(lua_State *L) {
	drawRect(video_addr(L),
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_rectfill(lua_State *L) {
	drawFilledRect(video_addr(L),
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_sprite(lua_State *L) {
	LuaObject *luaObj = (LuaObject *)lua_touserdata(L, 1);
	Bitmap *bmp = (Bitmap *)luaObj->handle;
	drawSprite(
		video_addr(L), bmp,
		(int)lua_tointeger(L, 2),
		(int)floor(lua_tonumber(L, 3)),
		(int)floor(lua_tonumber(L, 4)),
		(int)luaL_optinteger(L, 5, 1),
		(int)luaL_optinteger(L, 6, 1),
		(float)luaL_optnumber(L, 7, 1.0),
		(uint8_t)luaL_optinteger(L, 8, 0),
		(uint32_t)luaL_optinteger(L, 9, 0x00),
		(ColorMode)luaL_optinteger(L, 10, Additive)
	);
	return 0;
}

static int l_tiles(lua_State *L) {
	LuaObject *luaObj = (LuaObject *)lua_touserdata(L, 1);
	Bitmap *bmp = (Bitmap *)luaObj->handle;
	int x = lua_tointeger(L, 3);
	int y = lua_tointeger(L, 4);
	int w = lua_tointeger(L, 5);
	int h = lua_tointeger(L, 6);
	int pitch = lua_tointeger(L, 7);
	lua_len(L, 2);
	int len = lua_tointeger(L, -1);
	lua_pop(L, 1);
	int xCurrent = x;
	int i = 1;
	while (len--) {
		lua_rawgeti(L, 2, i);
		int sprIndex = lua_tointeger(L, -1);
		lua_pop(L, 1);
		if (sprIndex > -1)
			drawSprite(video_addr(L), bmp, sprIndex, xCurrent, y, w, h, 1, 0, 0x00, Additive);
		xCurrent += w * SPRITE_SIZE;
		if (i++ % pitch == 0) {
			xCurrent = x;
			y += h * SPRITE_SIZE;
		}
	}
	return 0;
}

static int l_font(lua_State *L) {
	Bitmap *bmp = NULL;
	LuaObject *luaObj = lua_touserdata(L, 1);
	if (luaObj) bmp = luaObj->handle;
	drawFont(video_addr(L), bmp,
		(int)floor(lua_tonumber(L, 2)),
		(int)floor(lua_tonumber(L, 3)),
		lua_tostring(L, 4),
		(int)floor(luaL_optinteger(L, 5, 1)),
		(uint32_t)floor(luaL_optinteger(L, 6, 0xffffff))
	);
	return 1;
}

static int l_wave(lua_State *L) {
	Wave *wave = loadWave(lua_tostring(L, 1));
	if (!wave)
		lua_pushnil(L);
	else {
		LuaObject *luaObj = lua_newuserdata(L, sizeof(LuaObject));
		luaObj->handle = wave;
		luaL_setmetatable(L, WAVE_META);
	}
	return 1;
}

static int l_wave_gc(lua_State *L) {
	LuaObject *luaObj = lua_touserdata(L, 1);
	Wave *wave = luaObj->handle;
	stopInstances(audio_addr(L), wave);
	freeWave(wave);
	return 0;
}

static int l_wavestream(lua_State *L) {
	const char *filePath = lua_tostring(L, 1);
	WaveStream *waveStream = openWaveStream(filePath);
	if (!waveStream)
		lua_pushnil(L);
	else {
		LuaObject *luaObj = lua_newuserdata(L, sizeof(LuaObject));
		luaObj->handle = waveStream;
		luaL_setmetatable(L, WAVESTREAM_META);
	}
	return 1;
}

static int l_wavestream_gc(lua_State *L) {
	LuaObject *luaObj = lua_touserdata(L, 1);
	WaveStream *waveStream = luaObj->handle;
	if (__getMilkFromReg(L)->audio.streamSlot.data == waveStream)
		stopStream(audio_addr(L));
	closeWaveStream(waveStream);
	return 0;
}

static int l_play(lua_State *L) {
	LuaObject *luaObj = lua_touserdata(L, 1);
	Wave *wave = luaObj->handle;
	playSound(audio_addr(L), wave, (int)lua_tointeger(L, 2), (int)lua_tointeger(L, 3));
	return 0;
}

static int l_pause(lua_State *L) {
  pauseSound(audio_addr(L),
    (int)lua_tointeger(L, 1)
  );
  return 0;
}

static int l_resume(lua_State *L) {
	resumeSound(audio_addr(L),
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_stop(lua_State *L) {
	stopSound(audio_addr(L),
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_sndslot(lua_State *L) {
	lua_pushinteger(L,
		getSoundState(audio_addr(L), (int)lua_tointeger(L, 1))
	);
	return 1;
}

static int l_playstream(lua_State *L) {
	LuaObject *luaObj = lua_touserdata(L, 1);
	WaveStream *waveStream = luaObj->handle;
  bool loop = false;
  if (lua_isboolean(L, 3)) loop = lua_toboolean(L, 3);
	playStream(audio_addr(L),
		waveStream,
		(int)lua_tointeger(L, 2),
    loop
	);
	return 0;
}

static int l_stopstream(lua_State *L) {
  UNUSED(L);
  stopStream(audio_addr(L));
  return 0;
}

static int l_pausestream(lua_State *L) {
  UNUSED(L);
  pauseStream(audio_addr(L));
  return 0;
}

static int l_resumestream(lua_State *L) {
  UNUSED(L);
  resumeStream(audio_addr(L));
  return 0;
}

static int l_vol(lua_State *L) {
	setMasterVolume(audio_addr(L), lua_tointeger(L, 1));
	return 0;
}

static int l_exit(lua_State *L) {
	UNUSED(L);
	__getMilkFromReg(L)->shouldQuit = true;
	return 0;
}

static void __register_milk(lua_State *L, Milk *milk) {
	lua_pushlightuserdata(L, (void *)&MilkRegistryKey);
	lua_pushlightuserdata(L, (void *)milk);
	lua_settable(L, LUA_REGISTRYINDEX);
}

static void __push_api_func(lua_State *L, const char *name, int(*api_func)(lua_State *)) {
	lua_pushcfunction(L, api_func);
	lua_setglobal(L, name);
}

static void __push_api(lua_State *L) {
	__push_api_func(L, "btn", l_btn);
	__push_api_func(L, "btnp", l_btnp);
	__push_api_func(L, "bitmap", l_bitmap);
	__push_api_func(L, "clip", l_clip);
	__push_api_func(L, "clrs", l_clrs);
	__push_api_func(L, "pset", l_pset);
	__push_api_func(L, "line", l_line);
	__push_api_func(L, "rect", l_rect);
	__push_api_func(L, "rectfill", l_rectfill);
	__push_api_func(L, "sprite", l_sprite);
	__push_api_func(L, "tiles", l_tiles);
	__push_api_func(L, "font", l_font);
	__push_api_func(L, "wave", l_wave);
	__push_api_func(L, "stream", l_wavestream);
	__push_api_func(L, "play", l_play);
  __push_api_func(L, "pause", l_pause);
	__push_api_func(L, "stop", l_stop);
	__push_api_func(L, "resume", l_resume);
	__push_api_func(L, "playstream", l_playstream);
  __push_api_func(L, "stopstream", l_stopstream);
  __push_api_func(L, "pausestream", l_pausestream);
  __push_api_func(L, "resumestream", l_resumestream);
	__push_api_func(L, "sndslot", l_sndslot);
	__push_api_func(L, "vol", l_vol);
	__push_api_func(L, "exit", l_exit);
}

static void __registerLuaObj(lua_State *L, const char* name, lua_CFunction gc) {
	luaL_newmetatable(L, name);
	lua_pushcfunction(L, gc);
	lua_setfield(L, -2, "__gc");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
}

void loadCode(Milk *milk) {
	if (milk->code.state == NULL) 	{
		lua_State *L = luaL_newstate();
		milk->code.state = (void *)L;
		luaL_openlibs(L);
		__register_milk(L, milk);
		__push_api(L);
		__registerLuaObj(L, BITMAP_META, l_bitmap_gc);
		__registerLuaObj(L, WAVE_META, l_wave_gc);
		__registerLuaObj(L, WAVESTREAM_META, l_wavestream_gc);
		if (luaL_dofile(L, "main.lua"))
			LOG_ERROR(lua_tostring(L, -1));
	}
}

void unloadCode(Milk *milk) {
	if (milk->code.state != NULL) {
		lua_close((lua_State *)milk->code.state);
		milk->code.state = NULL;
	}
}

void invokeInit(Code *code) {
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_init");
	if (lua_pcall(L, 0, 0, 0) != 0) {
		LOG_ERROR(lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}

void invokeUpdate(Code *code) {
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_update");
	if (lua_pcall(L, 0, 0, 0) != 0) {
		LOG_ERROR(lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}

void invokeDraw(Code *code) {
	lua_State *L = code->state;
	resetDrawState(video_addr(L));
	lua_getglobal(L, "_draw");
	if (lua_pcall(L, 0, 0, 0) != 0)	{
		LOG_ERROR(lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}
