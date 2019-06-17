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
		std::cout << "Error initializing SDL_Audio: " << SDL_GetError() << std::endl;
		return false;
	}

	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	if ((Mix_Init(flags) & flags) != flags) {
		std::cout << "Mix_Init: Failed to init required ogg and mp3 support: " << Mix_GetError() << std::endl;
		return false;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, STEREO_CHANNELS, MIX_CHUNK_SIZE) == -1) {
		std::cout << "SDL_Mixer: Error opening audio: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void milk::SDLAudioPlayer::playSound(Sound* sound) {
	if (Mix_PlayChannel(FIRST_AVAILABLE_CHANNEL, (Mix_Chunk*)sound->handle, 0) == -1) {
		std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
	}
}

void milk::SDLAudioPlayer::free() {
	Mix_CloseAudio();
	Mix_Quit();
}
