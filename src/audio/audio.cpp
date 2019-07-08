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
static const int NUM_CHANNELS = 32;
static const int MIX_CHUNK_SIZE = 2048;
static const int STEREO_CHANNELS = 2;
static const int INVALID_CHANNEL = -1;
static const int LOOP = -1;
static const int NO_LOOP = 0;

static float master_volume = 1.f;

/* Sound */
static std::array<milk::Sound*, NUM_CHANNELS> channels;
static std::array<float, NUM_CHANNELS> channel_volume;
static std::queue<int> free_channels;

/* Sound Cache*/
static const milk::u32 SND_UID_GEN_BITS = 16;
static const milk::u32 SND_UID_IDX_BITS = 16;
static const milk::u32 SND_UID_MAX_FREE_IDX = 1024;
static milk::UID sound_uids;
static std::unordered_map<std::string, milk::u32> sounds_by_path;
static std::unordered_map<milk::u32, milk::SoundData*> sounds_by_id;

/* Music */
static milk::Music* current_music = nullptr;

/* Music Cache*/
static const milk::u32 MUS_UID_GEN_BITS = 16;
static const milk::u32 MUS_UID_IDX_BITS = 16;
static const milk::u32 MUS_UID_MAX_FREE_IDX = 1024;
static milk::UID music_uids;
static std::unordered_map<std::string, milk::u32> music_by_path;
static std::unordered_map<milk::u32, milk::MusicData*> music_by_id;


/* Private */
static void on_channel_finished(int channelNum)
{
	milk::Sound* sound = channels[channelNum];
	sound->channel = INVALID_CHANNEL;
	sound->state = milk::SampleState::STOPPED;
	channels[channelNum] = nullptr;
	free_channels.push(channelNum);
}

static void on_music_finished()
{
	current_music->state = milk::SampleState::STOPPED;
	current_music = nullptr;
}

static void play_sound(milk::Sound* sound, float fadeTime, int loop)
{
	audio_stop_sound(sound, 0.f);

	int channelNum = free_channels.front();
	Mix_Volume(channelNum, (int)(sound->volume * (master_volume * MIX_MAX_VOLUME)));
	milk::SoundData* soundData = sounds_by_id.at(sound->uid);
	if (Mix_FadeInChannel(channelNum, (Mix_Chunk*)soundData->handle, loop, (int)(fadeTime * 1000)) == -1) {
		std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
		return;
	}

	free_channels.pop();
	sound->channel = channelNum;
	sound->state = milk::SampleState::PLAYING;
	channels[channelNum] = sound;
	channel_volume[channelNum] = sound->volume;
}

static void play_music(milk::Music* music, float fadeTime, int loop)
{
	Mix_HaltMusic();

	Mix_VolumeMusic((int)(music->volume * (master_volume * MIX_MAX_VOLUME)));
	milk::MusicData* musicData = music_by_id.at(music->uid);
	if (Mix_FadeInMusic((Mix_Music*)musicData->handle, loop, (int)(fadeTime * 1000)) == -1) {
		std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
		return;
	}

	music->state = milk::SampleState::PLAYING;
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

	for (auto itr : sounds_by_id) {
		id::recycle(&sound_uids, SND_UID_GEN_BITS, itr.first);
		Mix_FreeChunk((Mix_Chunk*)itr.second->handle);
		delete itr.second;
	}
	sounds_by_id.clear();
	sounds_by_path.clear();

	for (auto itr : music_by_id) {
		id::recycle(&music_uids, MUS_UID_GEN_BITS, itr.first);
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

	for (int i = 0; i < NUM_CHANNELS; ++i) {
		Mix_Volume(i, (int)(channel_volume[i] * v));
	}

	if (current_music != nullptr) {
		Mix_VolumeMusic((int)(current_music->volume * v));
	}
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
		++sounds_by_id.at(id)->refCount;
		return loaded->second;
	}

	Mix_Chunk* sample = Mix_LoadWAV(path);
	if (!sample) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		return id::INVALID_ID;
	}

	SoundData* soundData = new SoundData();
	soundData->path = path;
	soundData->handle = sample;
	soundData->refCount = 1;

	u32 id = id::make(&sound_uids, SND_UID_GEN_BITS, SND_UID_MAX_FREE_IDX);
	sounds_by_path.insert(std::make_pair(path, id));
	sounds_by_id.insert(std::make_pair(id, soundData));
	return id;
}

milk::SoundData* milk::audio_get_sounddata(u32 id)
{
	return id::valid(&sound_uids, SND_UID_GEN_BITS, SND_UID_IDX_BITS, id) ? sounds_by_id.at(id) : nullptr;
}

void milk::audio_dereference_sounddata(u32 id)
{
	if (id::valid(&sound_uids, SND_UID_GEN_BITS, SND_UID_IDX_BITS, id)) {
		milk::SoundData* soundData = sounds_by_id.at(id);
		if (--soundData->refCount <= 0) {
			sounds_by_path.erase(soundData->path);
			sounds_by_id.erase(id);
			Mix_FreeChunk((Mix_Chunk*)soundData->handle);
			delete soundData;

			id::recycle(&sound_uids, SND_UID_GEN_BITS, id);
		}
	}
}

milk::u32 milk::audio_load_musicdata(const char* path)
{
	std::unordered_map<std::string, u32>::iterator loaded = music_by_path.find(path);
	if (loaded != music_by_path.end()) {
		u32 id = loaded->second;
		++sounds_by_id.at(id)->refCount;
		return loaded->second;
	}

	Mix_Music* music = Mix_LoadMUS(path);
	if (!music) {
		std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
		return id::INVALID_ID;
	}

	MusicData* musicData = new MusicData();
	musicData->path = path;
	musicData->handle = music;
	musicData->refCount = 1;

	u32 id = id::make(&music_uids, MUS_UID_GEN_BITS, MUS_UID_MAX_FREE_IDX);
	music_by_path.insert(std::make_pair(path, id));
	music_by_id.insert(std::make_pair(id, musicData));
	return id;
}

milk::MusicData* milk::audio_get_musicdata(u32 id)
{
	return id::valid(&music_uids, MUS_UID_GEN_BITS, MUS_UID_IDX_BITS, id) ? music_by_id.at(id) : nullptr;
}

void milk::audio_dereference_musicdata(u32 id)
{
	if (id::valid(&music_uids, MUS_UID_GEN_BITS, MUS_UID_IDX_BITS, id)) {
		milk::MusicData* musicData = music_by_id.at(id);
		if (--musicData->refCount <= 0) {
			music_by_path.erase(musicData->path);
			music_by_id.erase(id);
			Mix_FreeMusic((Mix_Music*)musicData->handle);
			delete musicData;

			id::recycle(&music_uids, MUS_UID_GEN_BITS, id);
		}
	}
}

void milk::audio_play_sound(Sound* sound, float fadeTime)
{
	play_sound(sound, fadeTime, NO_LOOP);
}

void milk::audio_loop_sound(Sound* sound, float fadeTime)
{
	play_sound(sound, fadeTime, LOOP);
}

void milk::audio_pause_sound(Sound* sound)
{
	if (sound->state == SampleState::PLAYING) {
		sound->state = SampleState::PAUSED;
		Mix_Pause(sound->channel);
	}
}

void milk::audio_resume_sound(Sound* sound)
{
	if (sound->state == SampleState::PAUSED) {
		sound->state = SampleState::PLAYING;
		Mix_Resume(sound->channel);
	}
}

void milk::audio_stop_sound(Sound* sound, float fadeTime)
{
	if (sound->state != SampleState::STOPPED) {
		sound->state = SampleState::STOPPED;
		Mix_FadeOutChannel(sound->channel, (int)(fadeTime * 1000));
	}
}

void milk::audio_set_sound_volume(Sound* sound, float volume)
{
	sound->volume = std::min(std::max(volume, 0.f), 1.f);

	if (sound->channel != INVALID_CHANNEL) {
		float v = sound->volume * (master_volume * MIX_MAX_VOLUME);
		channel_volume[sound->channel] = v;
		Mix_Volume(sound->channel, (int)v);
	}
}

void milk::audio_play_music(Music* music, float fadeTime)
{
	play_music(music, fadeTime, NO_LOOP);
}

void milk::audio_loop_music(Music* music, float fadeTime)
{
	play_music(music, fadeTime, LOOP);
}

void milk::audio_pause_music(Music* music)
{
	if (music->state == SampleState::PLAYING) {
		music->state = SampleState::PAUSED;
		Mix_PauseMusic();
	}
}

void milk::audio_resume_music(Music* music)
{
	if (music->state == SampleState::PAUSED) {
		music->state = SampleState::PLAYING;
		Mix_ResumeMusic();
	}
}

void milk::audio_stop_music(Music* music, float fadeTime)
{
	if (music->state != SampleState::STOPPED) {
		music->state = SampleState::STOPPED;
		Mix_FadeOutMusic((int)(fadeTime * 1000));
	}
}

void milk::audio_set_music_volume(Music* music, float volume)
{
	music->volume = std::min(std::max(volume, 0.f), 1.f);

	if (music->state == SampleState::PLAYING) {
		Mix_VolumeMusic((int)(music->volume * (master_volume * MIX_MAX_VOLUME)));
	}
}
