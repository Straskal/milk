#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "int.h"

namespace milk 
{
	struct sound;
	struct sound_data;
	struct music;
	struct music_data;

	bool audio_init();
	void audio_quit();

	void audio_set_master_volume(float volume);
	float audio_get_master_volume();

	u32 audio_load_sounddata(const char* path);
	sound_data* audio_get_sounddata(u32 id);
	void audio_dereference_sounddata(u32 id);

	u32 audio_load_musicdata(const char* path);
	music_data* audio_get_musicdata(u32 id);
	void audio_dereference_musicdata(u32 id);

	void audio_play_sound(sound* sound, float fadetime);
	void audio_loop_sound(sound* sound, float fadetime);
	void audio_pause_sound(sound* sound);
	void audio_resume_sound(sound* sound);
	void audio_stop_sound(sound* sound, float fadetime);
	void audio_set_sound_volume(sound* sound, float volume);

	void audio_play_music(music* music, float fadetime);
	void audio_loop_music(music* music, float fadetime);
	void audio_pause_music(music* music);
	void audio_resume_music(music* music);
	void audio_stop_music(music* music, float fadetime);
	void audio_set_music_volume(music* music, float volume);
}

#endif