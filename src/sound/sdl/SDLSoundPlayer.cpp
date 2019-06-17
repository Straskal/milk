#include "SDLSoundPlayer.h"

#include <iostream>
#include <unordered_map>

#include "SDL.h"
#include "SDL_mixer.h"

#include "sound/Sound.h"

static const int MIX_CHUNK_SIZE = 2048;
static const int STEREO_CHANNELS = 2;
static const int FIRST_AVAILABLE_CHANNEL = -1;
static const int INVALID_CHANNEL = -1;
static const int NO_LOOP = 0;

static std::unordered_map<int, milk::SoundHandle*> channel_sound_map;

// Called when a sound has finished playing or is stopped.
static void on_channel_finished(int channel) {
	milk::SoundHandle* soundHandle = channel_sound_map.at(channel);
	channel_sound_map.erase(channel);
	soundHandle->channel = INVALID_CHANNEL;
}

bool milk::SDLSoundPlayer::init() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL_Init: Failed to initialize audio: " << SDL_GetError() << std::endl;
		return false;
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, STEREO_CHANNELS, MIX_CHUNK_SIZE) == -1) {
		std::cout << "SDL_Mixer: Failed to open audio: " << Mix_GetError() << std::endl;
		return false;
	}

	Mix_ChannelFinished(on_channel_finished);
	return true;
}

void milk::SDLSoundPlayer::playSound(SoundHandle* soundHandle) {
	// If the sound is currently playing, then stop it.
	if (soundHandle->channel != INVALID_CHANNEL) {
		Mix_HaltChannel(soundHandle->channel);
		soundHandle->channel = INVALID_CHANNEL;
	}

	Sound* sound = soundHandle->sound;
	int channel = Mix_PlayChannel(FIRST_AVAILABLE_CHANNEL, (Mix_Chunk*)sound->handle, NO_LOOP);
	// This can either be a fatal error or a failure to find an available channel.
	// If we ever have trouble finding an available channel, we may have to allocate one.
	if (channel == INVALID_CHANNEL) {
		std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
		return;
	}
	channel_sound_map.insert(std::make_pair(channel, soundHandle));
	soundHandle->channel = channel;
}

void milk::SDLSoundPlayer::stopSound(SoundHandle* soundHandle) {
	if (soundHandle->channel != INVALID_CHANNEL) {
		// This will call back to 'on_channel_finished', which resets this sound's channel to INVALID_CHANNEL.
		Mix_HaltChannel(soundHandle->channel);
	}
}

void milk::SDLSoundPlayer::free() {
	Mix_CloseAudio();
	Mix_Quit();
}
