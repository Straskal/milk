#include "audio.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "SDL.h"
#include "SDL_mixer.h"

#include "sample.h"
#include "uid.h"

/* Audio */
#define NUM_CHANNELS 32
#define MIX_CHUNK_SIZE 2048
#define STEREO_CHANNELS 2
#define INVALID_CHANNEL -1
#define LOOP -1
#define NO_LOOP 0

static float master_volume = 1.f;

/* Samples */
static std::array<milk::sound*, NUM_CHANNELS> channels;
static std::array<float, NUM_CHANNELS> channel_volume;
static std::queue<int> free_channels;
static milk::music* current_music = nullptr;

/* Sample Caches */
static milk::uid_data sample_uids;
static std::unordered_map<std::string, milk::u32> sounds_by_path;
static std::unordered_map<milk::u32, milk::sound_data*> sounds_by_id;
static std::unordered_map<std::string, milk::u32> music_by_path;
static std::unordered_map<milk::u32, milk::music_data*> music_by_id;

/* Private */
static void on_channel_finished(int channelnum)
{
	milk::sound* sound = channels[channelnum];
	sound->channel = INVALID_CHANNEL;
	sound->state = milk::sample_state::STOPPED;
	channels[channelnum] = nullptr;
	free_channels.push(channelnum);
}

static void on_music_finished()
{
	current_music->state = milk::sample_state::STOPPED;
	current_music = nullptr;
}

static void play_sound(milk::sound* sound, float fadetime, int loop)
{
	audio_stop_sound(sound, 0.f);

	int channel_num = free_channels.front();
	Mix_Volume(channel_num, (int)(sound->volume * (master_volume * MIX_MAX_VOLUME)));
	milk::sound_data* snddata = sounds_by_id.at(sound->uid);
	if (Mix_FadeInChannel(channel_num, (Mix_Chunk*)snddata->handle, loop, (int)(fadetime * 1000)) == -1) {
		std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
		return;
	}

	free_channels.pop();
	sound->channel = channel_num;
	sound->state = milk::sample_state::PLAYING;
	channels[channel_num] = sound;
	channel_volume[channel_num] = sound->volume;
}

static void play_music(milk::music* music, float fadetime, int loop)
{
	Mix_HaltMusic();

	Mix_VolumeMusic((int)(music->volume * (master_volume * MIX_MAX_VOLUME)));
	milk::music_data* musdata = music_by_id.at(music->uid);
	if (Mix_FadeInMusic((Mix_Music*)musdata->handle, loop, (int)(fadetime * 1000)) == -1) {
		std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
		return;
	}

	music->state = milk::sample_state::PLAYING;
	current_music = music;
}


/* Public */
bool milk::audio_init()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL_Init: Failed to initialize audio: " << SDL_GetError() << std::endl;
		return false;
	}

	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	if (Mix_Init(flags) != flags) {
		std::cout << "SDL_Mixer: Failed to initialize support for Ogg Vorbis and MP3: " << Mix_GetError() << std::endl;
		return false;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, STEREO_CHANNELS, MIX_CHUNK_SIZE) == -1) {
		std::cout << "SDL_Mixer: Failed to open audio: " << Mix_GetError() << std::endl;
		return false;
	}

	Mix_AllocateChannels(NUM_CHANNELS);
	for (int i = 0; i < NUM_CHANNELS; ++i) {
		channels[i] = nullptr;
		channel_volume[i] = 1.f;
		free_channels.push(i);
	}

	Mix_ChannelFinished(on_channel_finished);
	Mix_HookMusicFinished(on_music_finished);
	return true;
}

void milk::audio_quit()
{
	Mix_HaltChannel(-1);
	Mix_HaltMusic();

	for (std::pair<u32, sound_data*> itr : sounds_by_id) {
		uid_free(&sample_uids, itr.first);
		Mix_FreeChunk((Mix_Chunk*)itr.second->handle);
		delete itr.second;
	}
	sounds_by_id.clear();
	sounds_by_path.clear();

	for (std::pair<u32, music_data*> itr : music_by_id) {
		uid_free(&sample_uids, itr.first);
		Mix_FreeMusic((Mix_Music*)itr.second->handle);
		delete itr.second;
	}
	music_by_id.clear();
	music_by_path.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void milk::audio_set_master_volume(float volume)
{
	master_volume = std::min(std::max(volume, 0.f), 1.f);
	int v = (int)(master_volume * MIX_MAX_VOLUME);

	for (int i = 0; i < NUM_CHANNELS; ++i)
		Mix_Volume(i, (int)(channel_volume[i] * v));
	if (current_music != nullptr)
		Mix_VolumeMusic((int)(current_music->volume * v));
}

float milk::audio_get_master_volume()
{
	return master_volume;
}

milk::u32 milk::audio_load_sounddata(const char* path)
{
	std::unordered_map<std::string, u32>::iterator loaded = sounds_by_path.find(path);
	if (loaded != sounds_by_path.end()) {
		u32 id = loaded->second;
		++sounds_by_id.at(id)->ref_count;
		return loaded->second;
	}

	Mix_Chunk* sample = Mix_LoadWAV(path);
	if (!sample) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		return milk::INVALID_UID;
	}

	sound_data* snddata = new sound_data();
	snddata->path = path;
	snddata->handle = sample;
	snddata->ref_count = 1;

	u32 uid = uid_new(&sample_uids);
	sounds_by_path.insert(std::make_pair(path, uid));
	sounds_by_id.insert(std::make_pair(uid, snddata));
	return uid;
}

milk::sound_data* milk::audio_get_sounddata(u32 id)
{
	return uid_alive(&sample_uids, id) ? sounds_by_id.at(id) : nullptr;
}

void milk::audio_dereference_sounddata(u32 id)
{
	if (uid_alive(&sample_uids, id)) {
		milk::sound_data* snddata = sounds_by_id.at(id);
		if (--snddata->ref_count <= 0) {
			sounds_by_path.erase(snddata->path);
			sounds_by_id.erase(id);
			Mix_FreeChunk((Mix_Chunk*)snddata->handle);
			delete snddata;

			uid_free(&sample_uids, id);
		}
	}
}

milk::u32 milk::audio_load_musicdata(const char* path)
{
	std::unordered_map<std::string, u32>::iterator loaded = music_by_path.find(path);
	if (loaded != music_by_path.end()) {
		u32 id = loaded->second;
		++music_by_id.at(id)->ref_count;
		return loaded->second;
	}

	Mix_Music* sample = Mix_LoadMUS(path);
	if (!sample) {
		std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
		return milk::INVALID_UID;
	}

	music_data* musdata = new music_data();
	musdata->path = path;
	musdata->handle = sample;
	musdata->ref_count = 1;

	u32 uid = uid_new(&sample_uids);
	music_by_path.insert(std::make_pair(path, uid));
	music_by_id.insert(std::make_pair(uid, musdata));
	return uid;
}

milk::music_data* milk::audio_get_musicdata(u32 id)
{
	return uid_alive(&sample_uids, id) ? music_by_id.at(id) : nullptr;
}

void milk::audio_dereference_musicdata(u32 id)
{
	if (uid_alive(&sample_uids, id)) {
		milk::music_data* musdata = music_by_id.at(id);
		if (--musdata->ref_count <= 0) {
			music_by_path.erase(musdata->path);
			music_by_id.erase(id);
			Mix_FreeMusic((Mix_Music*)musdata->handle);
			delete musdata;

			uid_free(&sample_uids, id);
		}
	}
}

void milk::audio_play_sound(sound* sound, float fadetime)
{
	play_sound(sound, fadetime, NO_LOOP);
}

void milk::audio_loop_sound(sound* sound, float fadetime)
{
	play_sound(sound, fadetime, LOOP);
}

void milk::audio_pause_sound(sound* sound)
{
	if (sound->state == sample_state::PLAYING) {
		sound->state = sample_state::PAUSED;
		Mix_Pause(sound->channel);
	}
}

void milk::audio_resume_sound(sound* sound)
{
	if (sound->state == sample_state::PAUSED) {
		sound->state = sample_state::PLAYING;
		Mix_Resume(sound->channel);
	}
}

void milk::audio_stop_sound(sound* sound, float fadeTime)
{
	if (sound->state != sample_state::STOPPED) {
		sound->state = sample_state::STOPPED;
		Mix_FadeOutChannel(sound->channel, (int)(fadeTime * 1000));
	}
}

void milk::audio_set_sound_volume(sound* sound, float volume)
{
	sound->volume = std::min(std::max(volume, 0.f), 1.f);

	if (sound->channel != INVALID_CHANNEL) {
		float v = sound->volume * (master_volume * MIX_MAX_VOLUME);
		channel_volume[sound->channel] = v;
		Mix_Volume(sound->channel, (int)v);
	}
}

void milk::audio_play_music(music* music, float fadetime)
{
	play_music(music, fadetime, NO_LOOP);
}

void milk::audio_loop_music(music* music, float fadetime)
{
	play_music(music, fadetime, LOOP);
}

void milk::audio_pause_music(music* music)
{
	if (music->state == sample_state::PLAYING) {
		music->state = sample_state::PAUSED;
		Mix_PauseMusic();
	}
}

void milk::audio_resume_music(music* music)
{
	if (music->state == sample_state::PAUSED) {
		music->state = sample_state::PLAYING;
		Mix_ResumeMusic();
	}
}

void milk::audio_stop_music(music* music, float fadetime)
{
	if (music->state != sample_state::STOPPED) {
		music->state = sample_state::STOPPED;
		Mix_FadeOutMusic((int)(fadetime * 1000));
	}
}

void milk::audio_set_music_volume(music* music, float volume)
{
	music->volume = std::min(std::max(volume, 0.f), 1.f);

	if (music->state == sample_state::PLAYING)
		Mix_VolumeMusic((int)(music->volume * (master_volume * MIX_MAX_VOLUME)));
}
