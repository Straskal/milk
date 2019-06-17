#include "SDLAudioPlayer.h"

#include <iostream>

#include "SDL.h"
#include "SDL_mixer.h"

#include "audio/Sound.h"

static const int MIX_CHUNK_SIZE = 2048;
static const int STEREO_CHANNELS = 2;
static const int FIRST_AVAILABLE_CHANNEL = -1;

bool milk::SDLAudioPlayer::init() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL_Init: Failed to initialize audio: " << SDL_GetError() << std::endl;
		return false;
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, STEREO_CHANNELS, MIX_CHUNK_SIZE) == -1) {
		std::cout << "SDL_Mixer: Failed to open audio: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void milk::SDLAudioPlayer::playSound(Sound* sound) {
	int channel = Mix_PlayChannel(FIRST_AVAILABLE_CHANNEL, (Mix_Chunk*)sound->handle, 0);
	// This can either be a fatal error or a failure to find an available channel.
	// If we ever have trouble finding an available channel, we may have to allocate one.
	if (channel == -1) {
		std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
	}
}

void milk::SDLAudioPlayer::free() {
	Mix_CloseAudio();
	Mix_Quit();
}
