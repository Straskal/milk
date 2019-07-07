#include "luaopen_milk_audio.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "AudioPlayer.h"
#include "Music.h"
#include "MusicCache.h"
#include "Sound.h"
#include "SoundCache.h"
#include "luamlib.h"
#include "State.h"

static const char* SOUND_METATABLE = "milk.sound";

static int soundmeta_tostring(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	const char* path = sound->data->path.c_str();
	lua_pushstring(L, path);
	return 1;
}

static int soundmeta_gc(lua_State* L)
{
	if (milk::State::initialized) {
		milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
		// We do NOT want to release a sound from memory while it is playing.
		milk::State::audioPlayer->stopSound(sound, 0.f);
		milk::State::sounds->dereference(sound->data);	
	}
	return 0;
}

static int soundmeta_play(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::State::audioPlayer->playSound(sound, fadeTime);
	return 1;
}

static int soundmeta_loop(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::State::audioPlayer->loopSound(sound, fadeTime);
	return 1;
}

static int soundmeta_pause(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::State::audioPlayer->pauseSound(sound);
	return 1;
}

static int soundmeta_resume(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::State::audioPlayer->resumeSound(sound);
	return 1;
}

static int soundmeta_stop(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::State::audioPlayer->stopSound(sound, fadeTime);
	return 1;
}

static int soundmeta_get_state(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	lua_pushinteger(L, (int)sound->state);
	return 1;
}

static int soundmeta_get_volume(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float volume = sound->volume;
	lua_pushnumber(L, volume);
	return 1;
}

static int soundmeta_set_volume(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float volume = (float)luaL_checknumber(L, 2);
	milk::State::audioPlayer->setSoundVolume(sound, volume);
	return 0;
}

static const luaL_Reg soundmeta_funcs[] = {
	{ "__tostring", soundmeta_tostring },
	{ "__gc", soundmeta_gc },
	{ "play", soundmeta_play },
	{ "loop", soundmeta_loop },
	{ "pause", soundmeta_pause },
	{ "resume", soundmeta_resume },
	{ "stop", soundmeta_stop },
	{ "get_state", soundmeta_get_state },
	{ "get_volume", soundmeta_get_volume },
	{ "set_volume", soundmeta_set_volume },
	{ nullptr, nullptr }
};

static const char* MUSIC_METATABLE = "milk.music";

static int musicmeta_tostring(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	const char* path = music->data->path.c_str();
	lua_pushstring(L, path);
	return 1;
}

static int musicmeta_gc(lua_State* L)
{
	if (milk::State::initialized) {
		milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
		// We do NOT want to release music from memory while it is playing.
		milk::State::audioPlayer->stopMusic(music, 0);
		milk::State::music->dereference(music->data);	
	}
	return 0;
}

static int musicmeta_play(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::State::audioPlayer->playMusic(music, fadeTime);
	return 1;
}

static int musicmeta_loop(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::State::audioPlayer->loopMusic(music, fadeTime);
	return 1;
}

static int musicmeta_pause(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	milk::State::audioPlayer->pauseMusic(music);
	return 1;
}

static int musicmeta_resume(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	milk::State::audioPlayer->resumeMusic(music);
	return 1;
}

static int musicmeta_stop(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::State::audioPlayer->stopMusic(music, fadeTime);
	return 1;
}

static int musicmeta_get_state(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	lua_pushinteger(L, (int)music->state);
	return 1;
}

static int musicmeta_get_volume(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float volume = music->volume;
	lua_pushnumber(L, volume);
	return 1;
}

static int musicmeta_set_volume(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float volume = (float)luaL_checknumber(L, 2);
	milk::State::audioPlayer->setMusicVolume(music, volume);
	return 0;
}

static const luaL_Reg musicmeta_funcs[] = {
	{ "__tostring", musicmeta_tostring },
	{ "__gc", musicmeta_gc },
	{ "play", musicmeta_play },
	{ "loop", musicmeta_loop },
	{ "pause", musicmeta_pause },
	{ "resume", musicmeta_resume },
	{ "stop", musicmeta_stop },
	{ "get_state", musicmeta_get_state },
	{ "get_volume", musicmeta_get_volume },
	{ "set_volume", musicmeta_set_volume },
	{ nullptr, nullptr }
};

static int audio_new_sound(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::SoundData* soundData = milk::State::sounds->load(value);
		if (soundData != nullptr) {
			milk::Sound* sound = (milk::Sound*)lua_newuserdata(L, sizeof(milk::Sound));
			sound->data = soundData;
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

static int audio_new_music(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::MusicData* musicData = milk::State::music->load(value);
		if (musicData != nullptr) {
			milk::Music* music = (milk::Music*)lua_newuserdata(L, sizeof(milk::Music));
			music->data = musicData;
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

static int audio_get_master_volume(lua_State* L)
{
	float masterVolume = milk::State::audioPlayer->getMasterVolume();
	lua_pushnumber(L, masterVolume);
	return 1;
}

static int audio_set_master_volume(lua_State* L)
{
	float masterVolume = (float)luaL_checknumber(L, 1);
	milk::State::audioPlayer->setMasterVolume(masterVolume);
	return 0;
}

static const luaL_Reg audio_funcs[] = {
	{ "new_sound", audio_new_sound },
	{ "new_music", audio_new_music },
	{ "get_master_volume", audio_get_master_volume},
	{ "set_master_volume", audio_set_master_volume },
	{ nullptr, nullptr }
};

static const milk::luaM_Enum audio_states_enum[] = {
	{ "PLAYING", (int)milk::SampleState::PLAYING },
	{ "PAUSED", (int)milk::SampleState::PAUSED },
	{ "STOPPED", (int)milk::SampleState::STOPPED }
};

int luaopen_milk_audio(lua_State* L)
{
	milk::luaM_createmetatable(L, SOUND_METATABLE, soundmeta_funcs);
	milk::luaM_createmetatable(L, MUSIC_METATABLE, musicmeta_funcs);
	luaL_newlib(L, audio_funcs);
	luaM_setenumfield(L, -1, "states", audio_states_enum, sizeof(audio_states_enum));
	return 1;
}
