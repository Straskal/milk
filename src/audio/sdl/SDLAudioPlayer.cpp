#include "SDLAudioPlayer.h"

#include <array>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "SDL.h"
#include "SDL_mixer.h"

#include "audio/Music.h"
#include "audio/Sound.h"

static const int NUM_CHANNELS = 32;
static const int MIX_CHUNK_SIZE = 2048;
static const int STEREO_CHANNELS = 2;

static const int INVALID_CHANNEL = -1;

static const int LOOP = -1;
static const int NO_LOOP = 0;

enum PlaybackType {
	FIRE_AND_FORGET,
	CONTROLLED
};

struct Channel {
	PlaybackType type;
	milk::Sound* sound;
};

static int master_volume = MIX_MAX_VOLUME;

static std::array<Channel, NUM_CHANNELS> channels;
static std::array<int, NUM_CHANNELS> channel_volume;
static std::queue<int> free_channels;

static milk::Music* current_music = nullptr;

static void on_channel_finished(int cnum) {
	Channel* channel = &channels.at(cnum);
	milk::Sound* sound = channel->sound;
	if (channel->type == PlaybackType::CONTROLLED && channel->sound != nullptr) {
		sound->channel = INVALID_CHANNEL;
	}
	else if (channel->type == PlaybackType::FIRE_AND_FORGET) {
		// TODO
	}
	channel->sound = nullptr;
	free_channels.push(cnum);
}

static void on_music_finished() {
	current_music = nullptr;
}

bool milk::SDLAudioPlayer::init() {
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
		channels[i].type = PlaybackType::CONTROLLED;
		channels[i].sound = nullptr;
		channel_volume[i] = master_volume;
		free_channels.push(i);
	}

	Mix_ChannelFinished(on_channel_finished);
	Mix_HookMusicFinished(on_music_finished);
	return true;
}

void milk::SDLAudioPlayer::free() {
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void milk::SDLAudioPlayer::playSound(Sound* sound) {
	stopSound(sound);

	int cnum = free_channels.front();
	free_channels.pop();
	sound->channel = cnum;

	Channel* channel = &channels.at(cnum);
	channel->sound = sound;
	channel->type = PlaybackType::CONTROLLED;
	channel_volume[cnum] = (int)(sound->volume * master_volume);

	Mix_Volume(cnum, channel_volume[cnum]);
	if (Mix_PlayChannel(cnum, (Mix_Chunk*)sound->data->handle, NO_LOOP) == -1) {
		std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
		return;
	}
}

void milk::SDLAudioPlayer::stopSound(Sound* sound) {
	if (sound->channel != INVALID_CHANNEL) {
		Mix_HaltChannel(sound->channel);
	}
}

void milk::SDLAudioPlayer::setSoundVolume(Sound* sound, float volume) {
	sound->volume = volume;
	if (sound->channel != INVALID_CHANNEL) {
		channel_volume[sound->channel] = (int)(sound->volume * master_volume);;
		Mix_Volume(sound->channel, volume * MIX_MAX_VOLUME);
	}
}

void milk::SDLAudioPlayer::playMusic(Music* music, int fadeTime) {
	stopMusic(0);
	MusicData* data = music->data;
	if (Mix_FadeInMusic((Mix_Music*)data->handle, NO_LOOP, fadeTime) == -1) {
		std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
	}
	current_music = music;
}

void milk::SDLAudioPlayer::loopMusic(Music* music, int fadeTime) {
	stopMusic(0);
	MusicData* data = music->data;
	if (Mix_FadeInMusic((Mix_Music*)data->handle, LOOP, fadeTime) == -1) {
		std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
	}
	current_music = music;
}

void milk::SDLAudioPlayer::stopMusic(int fadeTime) {
	if (Mix_PlayingMusic()) {
		Mix_FadeOutMusic(fadeTime);
	}
}

bool milk::SDLAudioPlayer::isMusicPlaying(const Music* music) const {
	return music == current_music;
}
