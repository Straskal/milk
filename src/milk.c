#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>
#include <stdint.h>

#include "common.h"
#include "milk.h"
#include "logs.h"

#define BITMAP_META "bitmap"

typedef struct {
	void *handle;
} LuaObject;

static Milk *globalMilk;

Milk *createMilk() {
	Milk *milk = (Milk *)malloc(sizeof(Milk));
	milk->shouldQuit = false;
	milk->code.state = NULL;
	initInput(&milk->input);
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

static int l_bitmap(lua_State *L) {
	const char *filePath = lua_tostring(L, 1);
	Bitmap *bmp = loadBitmap(filePath);
	if (!bmp)
		lua_pushnil(L);
	else {
		LuaObject *luaObj = lua_newuserdata(L, sizeof(LuaObject));
		luaObj->handle = (void *)bmp;
		luaL_setmetatable(L, BITMAP_META);
	}
	return 1;
}

static int l_bitmap_gc(lua_State *L) {
	LuaObject *luaObj = (LuaObject *)lua_touserdata(L, 1);
	Bitmap *bmp = (Bitmap *)luaObj->handle;
	freeBitmap(bmp);
	return 0;
}

static int l_sprite(lua_State *L) {
	LuaObject *luaObj = (LuaObject *)lua_touserdata(L, 1);
	Bitmap *bmp = (Bitmap *)luaObj->handle;
	sprite(
		&globalMilk->video, bmp,
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
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	int w = lua_tointeger(L, 4);
	lua_len(L, 5);
	int len = lua_tointeger(L, -1);
	lua_pop(L, 1);
	int xCurrent = x;
	int i = 1;
	while (len--) {
		lua_rawgeti(L, 5, i);
		int sprIndex = lua_tointeger(L, -1);
		lua_pop(L, 1);
		sprite(&globalMilk->video, bmp, sprIndex, xCurrent, y, 2, 2, 1, 0, 0x00, Additive);
		xCurrent += 16;
		if (i++ % w == 0) {
			xCurrent = x;
			y += 16;
		}
	}

	return 0;
}

static int l_btn(lua_State *L) {
	lua_pushboolean(L,
		isButtonDown(&globalMilk->input,
		(ButtonState)(1 << lua_tointeger(L, 1))));
	return 1;
}

static int l_btnp(lua_State *L) {
	lua_pushboolean(L,
		isButtonPressed(&globalMilk->input,
		(ButtonState)(1 << lua_tointeger(L, 1))));
	return 1;
}

static int l_clip(lua_State *L) {
	clip(&globalMilk->video,
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4)
	);
	return 0;
}

static int l_clrs(lua_State *L) {
	clearFramebuffer(&globalMilk->video,
		(uint32_t)luaL_optinteger(L, 1, 0x000000));
	return 0;
}

static int l_pset(lua_State *L) {
	pixel(&globalMilk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(uint32_t)lua_tointeger(L, 3)
	);
	return 0;
}

static int l_line(lua_State *L) {
	line(&globalMilk->video,
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_rect(lua_State *L) {
	rect(&globalMilk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_rectfill(lua_State *L) {
	rectFill(&globalMilk->video,
		(int)floor(lua_tonumber(L, 1)),
		(int)floor(lua_tonumber(L, 2)),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(uint32_t)lua_tointeger(L, 5)
	);
	return 0;
}

static int l_font(lua_State *L) {
	Bitmap *bmp = NULL;

	LuaObject *luaObj = lua_touserdata(L, 1);
	if (luaObj) bmp = luaObj->handle;

	font(&globalMilk->video,
		bmp,
		(int)floor(lua_tonumber(L, 2)),
		(int)floor(lua_tonumber(L, 3)),
		lua_tostring(L, 4),
		(int)floor(luaL_optinteger(L, 5, 1)),
		(uint32_t)floor(luaL_optinteger(L, 6, 0xffffff))
	);
	return 1;
}

static int l_loadsnd(lua_State *L) {
	loadSound(&globalMilk->audio,
		(int)lua_tointeger(L, 1),
		lua_tostring(L, 2)
	);
	return 0;
}

static int l_freesnd(lua_State *L) {
	unloadSound(&globalMilk->audio,
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_play(lua_State *L) {
	playSound(&globalMilk->audio,
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3)
	);
	return 0;
}

static int l_pause(lua_State *L) {
  pauseSound(&globalMilk->audio,
    (int)lua_tointeger(L, 1)
  );
  return 0;
}

static int l_resume(lua_State *L) {
	resumeSound(&globalMilk->audio,
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_stop(lua_State *L) {
	stopSound(&globalMilk->audio,
		(int)lua_tointeger(L, 1)
	);
	return 0;
}

static int l_sndslot(lua_State *L) {
	lua_pushinteger(L,
		getSoundState(&globalMilk->audio, (int)lua_tointeger(L, 1))
	);
	return 1;
}

static int l_openstream(lua_State *L) {
	openStream(&globalMilk->audio,
		(int)lua_tointeger(L, 1),
		lua_tostring(L, 2)
	);
	return 0;
}

static int l_closestream(lua_State *L) {
  closeStream(&globalMilk->audio,
		(int)lua_tointeger(L, 1)
  );
  return 0;
}

static int l_playstream(lua_State *L) {
  bool loop = false;
  if (lua_isboolean(L, 3)) loop = lua_toboolean(L, 3);
	playStream(&globalMilk->audio,
		(int)lua_tointeger(L, 1),
		(int)lua_tointeger(L, 2),
    loop
	);
	return 0;
}

static int l_stopstream(lua_State *L) {
  UNUSED(L);
  stopStream(&globalMilk->audio, lua_tointeger(L, 1));
  return 0;
}

static int l_pausestream(lua_State *L) {
  UNUSED(L);
  pauseStream(&globalMilk->audio, lua_tointeger(L, 1));
  return 0;
}

static int l_resumestream(lua_State *L) {
  UNUSED(L);
  resumeStream(&globalMilk->audio, lua_tointeger(L, 1));
  return 0;
}

static int l_vol(lua_State *L) {
	setMasterVolume(&globalMilk->audio,
		lua_tointeger(L, 1)
	);
	return 0;
}

static int l_exit(lua_State *L) {
	UNUSED(L);
	globalMilk->shouldQuit = true;
	return 0;
}

static void pushApiFunction(lua_State *L, const char *name, int(*api_func)(lua_State *)) {
	lua_pushcfunction(L, api_func);
	lua_setglobal(L, name);
}

static void pushApi(lua_State *L) {
	pushApiFunction(L, "bitmap", l_bitmap);
	pushApiFunction(L, "sprite", l_sprite);
	pushApiFunction(L, "tiles", l_tiles);
	pushApiFunction(L, "btn", l_btn);
	pushApiFunction(L, "btnp", l_btnp);
	pushApiFunction(L, "clip", l_clip);
	pushApiFunction(L, "clrs", l_clrs);
	pushApiFunction(L, "pset", l_pset);
	pushApiFunction(L, "line", l_line);
	pushApiFunction(L, "rect", l_rect);
	pushApiFunction(L, "rectfill", l_rectfill);
	pushApiFunction(L, "font", l_font);
	pushApiFunction(L, "loadsnd", l_loadsnd);
	pushApiFunction(L, "freesnd", l_freesnd);
	pushApiFunction(L, "play", l_play);
  pushApiFunction(L, "pause", l_pause);
	pushApiFunction(L, "stop", l_stop);
	pushApiFunction(L, "resume", l_resume);
	pushApiFunction(L, "openstream", l_openstream);
  pushApiFunction(L, "closestream", l_closestream);
	pushApiFunction(L, "playstream", l_playstream);
  pushApiFunction(L, "stopstream", l_stopstream);
  pushApiFunction(L, "pausestream", l_pausestream);
  pushApiFunction(L, "resumestream", l_resumestream);
	pushApiFunction(L, "sndslot", l_sndslot);
	pushApiFunction(L, "vol", l_vol);
	pushApiFunction(L, "exit", l_exit);
}

void loadCode(Milk *milk) {
	if (milk->code.state == NULL) 	{
		lua_State *L = luaL_newstate();
		globalMilk = milk;
		milk->code.state = (void *)L;
		luaL_openlibs(L);
		pushApi(L);
		luaL_newmetatable(L, BITMAP_META);
		lua_pushcfunction(L, l_bitmap_gc);
		lua_setfield(L, -2, "__gc");
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		lua_pop(L, 1);
		if (luaL_dofile(L, "main.lua"))
			LOG_ERROR(lua_tostring(L, -1));
	}
}

void unloadCode(Milk *milk) {
	if (milk->code.state != NULL) {
		lua_close((lua_State *)milk->code.state);
		milk->code.state = NULL;
		globalMilk = NULL;
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
	resetDrawState(&globalMilk->video);
	lua_State *L = (lua_State *)code->state;
	lua_getglobal(L, "_draw");
	if (lua_pcall(L, 0, 0, 0) != 0)	{
		LOG_ERROR(lua_tostring(L, -1));
		lua_pop(L, -1);
	}
}
