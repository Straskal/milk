#include "luaopen_milk_audio.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Music.h"
#include "MusicCache.h"
#include "Sound.h"
#include "SoundCache.h"
#include "core/Locator.h"
#include "core/luamlib.h"

static const char* SOUND_METATABLE = "milk.sound";

static int soundmeta_gc(lua_State* L) {
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	// Stop sound before derefencing it. We do NOT want to release a sound from memory while it is playing.
	milk::Locator::audioPlayer->stopSound(sound);
	milk::Locator::sounds->dereference(sound->data);
	return 0;
}

static const luaL_Reg soundmeta_funcs[] = {
	{ "__gc", soundmeta_gc },
	{ NULL, NULL }
};

static const char* MUSIC_METATABLE = "milk.music";

static int musicmeta_gc(lua_State* L) {
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	// Stop music before derefencing it. We do NOT want to release music from memory while it is playing.
	milk::Locator::audioPlayer->stopMusic(0);
	milk::Locator::music->dereference(music->data);
	return 0;
}

static const luaL_Reg musicmeta_funcs[] = {
	{ "__gc", musicmeta_gc },
	{ NULL, NULL }
};

static int audio_new_sound(lua_State* L) {
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

static int audio_play_sound(lua_State* L) {
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::Locator::audioPlayer->playSound(sound);
	return 0;
}

static int audio_stop_sound(lua_State* L) {
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::Locator::audioPlayer->stopSound(sound);
	return 0;
}

static int audio_new_music(lua_State* L) {
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::MusicData* musicData = milk::Locator::music->load(value);
		if (musicData != nullptr) {
			milk::Music* music = (milk::Music*)lua_newuserdata(L, sizeof(milk::Music*));
			luaL_getmetatable(L, MUSIC_METATABLE);
			lua_setmetatable(L, -2);
			music->data = musicData;
			lua_pushboolean(L, true);
			return 2;
		}
	}
	lua_pushnil(L);
	lua_pushboolean(L, false);
	return 2;
}

static int audio_play_music(lua_State* L) {
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	int fadeTime = (int)(luaL_optnumber(L, 2, 0) * 1000);
	milk::Locator::audioPlayer->playMusic(music, fadeTime);
	return 0;
}

static int audio_loop_music(lua_State* L) {
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	int fadeTime = (int)(luaL_optnumber(L, 2, 0) * 1000);
	milk::Locator::audioPlayer->loopMusic(music, fadeTime);
	return 0;
}

static int audio_stop_music(lua_State* L) {
	int fadeTime = (int)(luaL_optnumber(L, 1, 0) * 1000);
	milk::Locator::audioPlayer->stopMusic(fadeTime);
	return 0;
}

static const luaL_Reg audio_funcs[] = {
	{ "new_sound", audio_new_sound },
	{ "play_sound", audio_play_sound },
	{ "stop_sound", audio_stop_sound },
	{ "new_music", audio_new_music },
	{ "play_music", audio_play_music },
	{ "loop_music", audio_loop_music },
	{ "stop_music", audio_stop_music },
	{ NULL, NULL }
};

int milk::luaopen_milk_audio(lua_State* L) {
	luaM_createmetatable(L, SOUND_METATABLE, soundmeta_funcs);
	luaM_createmetatable(L, MUSIC_METATABLE, musicmeta_funcs);
	luaL_newlib(L, audio_funcs);
	return 1;
}
