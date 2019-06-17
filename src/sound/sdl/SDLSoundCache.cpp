#include "SDLSoundCache.h"

#include <iostream>

#include "SDL.h"
#include "SDL_mixer.h"

#include "sound/Sound.h"
#include "sound/sdl/SDLSoundPlayer.h"

milk::SDLSoundCache::SDLSoundCache(SDLSoundPlayer* soundPlayer)
	: m_soundPlayer{ soundPlayer } {
}

milk::Sound* milk::SDLSoundCache::load(const std::string& path) {
	auto found = m_sounds.find(path);
	if (found != m_sounds.end()) {
		++found->second->refCount;
		return found->second;
	}

	Mix_Chunk* sample = Mix_LoadWAV(path.c_str());
	if (!sample) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		return nullptr;
	}

	Sound* sound = new Sound();
	sound->path = path;
	sound->handle = sample;
	sound->refCount = 1;

	m_sounds.insert(std::make_pair(path, sound));
	return sound;
}

void milk::SDLSoundCache::dereference(SoundHandle* soundHandle) {
	Sound* sound = soundHandle->sound;
	if (--sound->refCount <= 0) {
		m_sounds.erase(sound->path);
		// We do NOT want to free a chunk that is currently playing.
		m_soundPlayer->stopSound(soundHandle);
		Mix_FreeChunk((Mix_Chunk*)sound->handle);
		delete sound; sound = nullptr;
	}
}

void milk::SDLSoundCache::free() {
	SDL_assert(m_sounds.size() == 0);
}
