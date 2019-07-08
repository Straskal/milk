#include "luaopen_milk_audio.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "audio.h"
#include "luamlib.h"
#include "sample.h"
#include "uid.h"

static const char* SOUND_METATABLE = "milk.sound";

static int lua_soundmeta_tostring(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::SoundData* soundData = milk::audio_get_sounddata(sound->uid);
	const char* path = soundData->path.c_str();
	lua_pushstring(L, path);
	return 1;
}

static int lua_soundmeta_gc(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	// We do NOT want to release a sound from memory while it is playing.
	milk::audio_stop_sound(sound, 0.f);
	milk::audio_dereference_sounddata(sound->uid);
	return 0;
}

static int lua_soundmeta_play(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::audio_play_sound(sound, fadeTime);
	return 1;
}

static int lua_soundmeta_loop(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::audio_loop_sound(sound, fadeTime);
	return 1;
}

static int lua_soundmeta_pause(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::audio_pause_sound(sound);
	return 1;
}

static int lua_soundmeta_resume(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::audio_resume_sound(sound);
	return 1;
}

static int lua_soundmeta_stop(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::audio_stop_sound(sound, fadeTime);
	return 1;
}

static int lua_soundmeta_get_state(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	lua_pushinteger(L, (int)sound->state);
	return 1;
}

static int lua_soundmeta_get_volume(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float volume = sound->volume;
	lua_pushnumber(L, volume);
	return 1;
}

static int lua_soundmeta_set_volume(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float volume = (float)luaL_checknumber(L, 2);
	milk::audio_set_sound_volume(sound, volume);
	return 0;
}

static const luaL_Reg lua_soundmeta_funcs[] = {
	{ "__tostring", lua_soundmeta_tostring },
	{ "__gc", lua_soundmeta_gc },
	{ "play", lua_soundmeta_play },
	{ "loop", lua_soundmeta_loop },
	{ "pause", lua_soundmeta_pause },
	{ "resume", lua_soundmeta_resume },
	{ "stop", lua_soundmeta_stop },
	{ "get_state", lua_soundmeta_get_state },
	{ "get_volume", lua_soundmeta_get_volume },
	{ "set_volume", lua_soundmeta_set_volume },
	{ nullptr, nullptr }
};

static const char* MUSIC_METATABLE = "milk.music";

static int lua_musicmeta_tostring(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	milk::MusicData* musicData = milk::audio_get_musicdata(music->uid);
	const char* path = musicData->path.c_str();
	lua_pushstring(L, path);
	return 1;
}

static int lua_musicmeta_gc(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	// We do NOT want to release music from memory while it is playing.
	milk::audio_stop_music(music, 0.f);
	milk::audio_dereference_musicdata(music->uid);
	return 0;
}

static int lua_musicmeta_play(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::audio_play_music(music, fadeTime);
	return 1;
}

static int lua_musicmeta_loop(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::audio_loop_music(music, fadeTime);
	return 1;
}

static int lua_musicmeta_pause(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	milk::audio_pause_music(music);
	return 1;
}

static int lua_musicmeta_resume(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	milk::audio_resume_music(music);
	return 1;
}

static int lua_musicmeta_stop(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::audio_stop_music(music, fadeTime);
	return 1;
}

static int lua_musicmeta_get_state(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	lua_pushinteger(L, (int)music->state);
	return 1;
}

static int lua_musicmeta_get_volume(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float volume = music->volume;
	lua_pushnumber(L, volume);
	return 1;
}

static int lua_musicmeta_set_volume(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float volume = (float)luaL_checknumber(L, 2);
	milk::audio_set_music_volume(music, volume);
	return 0;
}

static const luaL_Reg lua_musicmeta_funcs[] = {
	{ "__tostring", lua_musicmeta_tostring },
	{ "__gc", lua_musicmeta_gc },
	{ "play", lua_musicmeta_play },
	{ "loop", lua_musicmeta_loop },
	{ "pause", lua_musicmeta_pause },
	{ "resume", lua_musicmeta_resume },
	{ "stop", lua_musicmeta_stop },
	{ "get_state", lua_musicmeta_get_state },
	{ "get_volume", lua_musicmeta_get_volume },
	{ "set_volume", lua_musicmeta_set_volume },
	{ nullptr, nullptr }
};

static int lua_audio_new_sound(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* path = lua_tostring(L, 1);
		milk::u32 uid = milk::audio_load_sounddata(path);
		if (uid != milk::INVALID_UID) {
			milk::Sound* sound = (milk::Sound*)lua_newuserdata(L, sizeof(milk::Sound));
			sound->uid = uid;
			sound->state = milk::SampleState::STOPPED;
			sound->channel = -1;
			sound->volume = 1.0f;
			luaL_getmetatable(L, SOUND_METATABLE);
			lua_setmetatable(L, -2);
			lua_pushboolean(L, true);
			return 2;
		}
	}
	lua_pushnil(L);
	lua_pushboolean(L, false);
	return 2;
}

static int lua_audio_new_music(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* path = lua_tostring(L, 1);
		milk::u32 uid = milk::audio_load_musicdata(path);
		if (uid != milk::INVALID_UID) {
			milk::Music* music = (milk::Music*)lua_newuserdata(L, sizeof(milk::Music));
			music->uid = uid;
			music->state = milk::SampleState::STOPPED;
			music->volume = 1.0f;
			luaL_getmetatable(L, MUSIC_METATABLE);
			lua_setmetatable(L, -2);
			lua_pushboolean(L, true);
			return 2;
		}
	}
	lua_pushnil(L);
	lua_pushboolean(L, false);
	return 2;
}

static int lua_audio_get_master_volume(lua_State* L)
{
	float masterVolume = milk::audio_get_master_volume();
	lua_pushnumber(L, masterVolume);
	return 1;
}

static int lua_audio_set_master_volume(lua_State* L)
{
	float masterVolume = (float)luaL_checknumber(L, 1);
	milk::audio_set_master_volume(masterVolume);
	return 0;
}

static const luaL_Reg lua_audio_funcs[] = {
	{ "new_sound", lua_audio_new_sound },
	{ "new_music", lua_audio_new_music },
	{ "get_master_volume", lua_audio_get_master_volume},
	{ "set_master_volume", lua_audio_set_master_volume },
	{ nullptr, nullptr }
};

static const milk::luaM_Enum lua_audio_states_enum[] = {
	{ "PLAYING", (int)milk::SampleState::PLAYING },
	{ "PAUSED", (int)milk::SampleState::PAUSED },
	{ "STOPPED", (int)milk::SampleState::STOPPED }
};

int luaopen_milk_audio(lua_State* L)
{
	milk::luaM_createmetatable(L, SOUND_METATABLE, lua_soundmeta_funcs);
	milk::luaM_createmetatable(L, MUSIC_METATABLE, lua_musicmeta_funcs);
	luaL_newlib(L, lua_audio_funcs);
	luaM_setenumfield(L, -1, "states", lua_audio_states_enum, sizeof(lua_audio_states_enum));
	return 1;
}
