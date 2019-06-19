#include "luaopen_milk_audio.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Sound.h"
#include "SoundCache.h"
#include "core/Locator.h"
#include "core/luamlib.h"

static const char* SOUND_METATABLE = "milk.sound";

static int gc(lua_State* L) {
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	// Stop sound before derefencing it. We do NOT want to release a sound from memory while it is playing.
	milk::Locator::audioPlayer->stopSound(sound);
	milk::Locator::sounds->dereference(sound->data);
	return 0;
}

static const luaL_Reg soundmeta_funcs[] = {
	{ "__gc", gc },
	{ NULL, NULL }
};

static int new_sound(lua_State* L) {
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::SoundData* soundData = milk::Locator::sounds->load(value);
		if (soundData != nullptr) {
			milk::Sound* sound = (milk::Sound*)lua_newuserdata(L, sizeof(milk::Sound*));
			luaL_getmetatable(L, SOUND_METATABLE);
			lua_setmetatable(L, -2);
			sound->data = soundData;
			sound->channel = -1;
			lua_pushboolean(L, true);
			return 2;
		}
	}
	lua_pushnil(L);
	lua_pushboolean(L, false);
	return 2;
}

static int play_sound(lua_State* L) {
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::Locator::audioPlayer->playSound(sound);
	return 0;
}

static int stop_sound(lua_State* L) {
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::Locator::audioPlayer->stopSound(sound);
	return 0;
}

static const luaL_Reg audio_funcs[] = {
	{ "new_sound", new_sound },
	{ "play_sound", play_sound },
	{ "stop_sound", stop_sound },
	{ NULL, NULL }
};

int milk::luaopen_milk_audio(lua_State* L) {
	luaM_createmetatable(L, SOUND_METATABLE, soundmeta_funcs);
	luaL_newlib(L, audio_funcs);
	return 1;
}
