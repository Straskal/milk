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
#include "core/Locator.h"
#include "core/luamlib.h"

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
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	// We do NOT want to release a sound from memory while it is playing.
	milk::Locator::audioPlayer->stopSound(sound, 0.f);
	milk::Locator::sounds->dereference(sound->data);
	return 0;
}

static int soundmeta_play(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::Locator::audioPlayer->playSound(sound, fadeTime);
	return 1;
}

static int soundmeta_loop(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::Locator::audioPlayer->loopSound(sound, fadeTime);
	return 1;
}

static int soundmeta_pause(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::Locator::audioPlayer->pauseSound(sound);
	return 1;
}

static int soundmeta_resume(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	milk::Locator::audioPlayer->resumeSound(sound);
	return 1;
}

static int soundmeta_stop(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::Locator::audioPlayer->stopSound(sound, fadeTime);
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
	milk::Locator::audioPlayer->setSoundVolume(sound, volume);
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
	{ NULL, NULL }
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
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	// We do NOT want to release music from memory while it is playing.
	milk::Locator::audioPlayer->stopMusic(music, 0);
	milk::Locator::music->dereference(music->data);
	return 0;
}

static int musicmeta_play(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::Locator::audioPlayer->playMusic(music, fadeTime);
	return 1;
}

static int musicmeta_loop(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::Locator::audioPlayer->loopMusic(music, fadeTime);
	return 1;
}

static int musicmeta_pause(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	milk::Locator::audioPlayer->pauseMusic(music);
	return 1;
}

static int musicmeta_resume(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	milk::Locator::audioPlayer->resumeMusic(music);
	return 1;
}

static int musicmeta_stop(lua_State* L)
{
	milk::Music* music = (milk::Music*)luaL_checkudata(L, 1, MUSIC_METATABLE);
	float fadeTime = (float)luaL_optnumber(L, 2, 0.f);
	milk::Locator::audioPlayer->stopMusic(music, fadeTime);
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
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float volume = sound->volume;
	lua_pushnumber(L, volume);
	return 1;
}

static int musicmeta_set_volume(lua_State* L)
{
	milk::Sound* sound = (milk::Sound*)luaL_checkudata(L, 1, SOUND_METATABLE);
	float volume = (float)luaL_checknumber(L, 2);
	milk::Locator::audioPlayer->setSoundVolume(sound, volume);
	return 0;
}

static const luaL_Reg musicmeta_funcs[] = {
	{ "__tostring", musicmeta_tostring },
	{ "__gc", musicmeta_gc },
	{ "play", soundmeta_play },
	{ "loop", soundmeta_loop },
	{ "pause", soundmeta_pause },
	{ "resume", soundmeta_resume },
	{ "stop", soundmeta_stop },
	{ "get_state", soundmeta_get_state },
	{ "get_volume", soundmeta_get_volume },
	{ "set_volume", soundmeta_set_volume },
	{ NULL, NULL }
};

static int audio_new_sound(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::SoundData* soundData = milk::Locator::sounds->load(value);
		if (soundData != nullptr) {
			milk::Sound* sound = (milk::Sound*)lua_newuserdata(L, sizeof(milk::Sound));
			sound->data = soundData;
			sound->state = milk::AudioState::STOPPED;
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
		milk::MusicData* musicData = milk::Locator::music->load(value);
		if (musicData != nullptr) {
			milk::Music* music = (milk::Music*)lua_newuserdata(L, sizeof(milk::Music));
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

static int audio_get_master_volume(lua_State* L)
{
	float masterVolume = milk::Locator::audioPlayer->getMasterVolume();
	lua_pushnumber(L, masterVolume);
	return 1;
}

static int audio_set_master_volume(lua_State* L)
{
	float masterVolume = (float)luaL_checknumber(L, 1);
	milk::Locator::audioPlayer->setMasterVolume(masterVolume);
	return 0;
}

static const luaL_Reg audio_funcs[] = {
	{ "new_sound", audio_new_sound },
	{ "new_music", audio_new_music },
	{ "get_master_volume", audio_get_master_volume},
	{ "set_master_volume", audio_set_master_volume },
	{ NULL, NULL }
};

static const milk::luaM_Enum audio_states_enum[] = {
	{ "PLAYING", (int)milk::AudioState::PLAYING },
	{ "PAUSED", (int)milk::AudioState::PAUSED },
	{ "STOPPED", (int)milk::AudioState::STOPPED }
};

int milk::luaopen_milk_audio(lua_State* L)
{
	luaM_createmetatable(L, SOUND_METATABLE, soundmeta_funcs);
	luaM_createmetatable(L, MUSIC_METATABLE, musicmeta_funcs);
	luaL_newlib(L, audio_funcs);
	luaM_setenumfield(L, -1, "states", audio_states_enum, sizeof(audio_states_enum) / sizeof(luaM_Enum));
	return 1;
}
