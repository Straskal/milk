#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "int.h"

namespace milk 
{
	struct Sound;
	struct SoundData;
	struct Music;
	struct MusicData;

	bool audio_init();
	void audio_quit();

	void audio_set_master_volume(float volume);
	float audio_get_master_volume();

	u32 audio_load_sounddata(const char* path);
	SoundData* audio_get_sounddata(u32 id);
	void audio_dereference_sounddata(u32 id);

	u32 audio_load_musicdata(const char* path);
	MusicData* audio_get_musicdata(u32 id);
	void audio_dereference_musicdata(u32 id);

	void audio_play_sound(Sound* sound, float fadeTime);
	void audio_loop_sound(Sound* sound, float fadeTime);
	void audio_pause_sound(Sound* sound);
	void audio_resume_sound(Sound* sound);
	void audio_stop_sound(Sound* sound, float fadeTime);
	void audio_set_sound_volume(Sound* sound, float volume);

	void audio_play_music(Music* music, float fadeTime);
	void audio_loop_music(Music* music, float fadeTime);
	void audio_pause_music(Music* music);
	void audio_resume_music(Music* music);
	void audio_stop_music(Music* music, float fadeTime);
	void audio_set_music_volume(Music* music, float volume);
}

#endif