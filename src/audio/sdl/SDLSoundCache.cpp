#include "SDLSoundCache.h"

#include <iostream>

#include "SDL.h"
#include "SDL_mixer.h"

#include "audio/Sound.h"

milk::SoundData* milk::SDLSoundCache::load(const std::string& path) {
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

	SoundData* sound = new SoundData();
	sound->path = path;
	sound->handle = sample;
	sound->refCount = 1;

	m_sounds.insert(std::make_pair(path, sound));
	return sound;
}

void milk::SDLSoundCache::dereference(SoundData* soundData) {
	if (--soundData->refCount <= 0) {
		m_sounds.erase(soundData->path);
		Mix_FreeChunk((Mix_Chunk*)soundData->handle);
		delete soundData; soundData = nullptr;
	}
}

void milk::SDLSoundCache::free() {
	SDL_assert(m_sounds.size() == 0);
}
