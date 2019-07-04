#include "SDLSoundCache.h"

#include <iostream>

#include "SDL.h"
#include "SDL_mixer.h"

#include "audio/Sound.h"

milk::SoundData* milk::SDLSoundCache::load(const char* path)
{
	std::unordered_map<std::string, SoundData*>::iterator loaded = m_sounds.find(path);
	if (loaded != m_sounds.end()) {
		++loaded->second->refCount;
		return loaded->second;
	}

	Mix_Chunk* sample = Mix_LoadWAV(path);
	if (!sample) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		return nullptr;
	}

	SoundData* soundData = new SoundData();
	soundData->path = path;
	soundData->handle = sample;
	soundData->refCount = 1;

	m_sounds.insert(std::make_pair(path, soundData));
	return soundData;
}

void milk::SDLSoundCache::dereference(SoundData* soundData)
{
	if (--soundData->refCount <= 0) {
		m_sounds.erase(soundData->path);
		Mix_FreeChunk((Mix_Chunk*)soundData->handle);
		delete soundData;
		soundData = nullptr;
	}
}

void milk::SDLSoundCache::free()
{
	SDL_assert(m_sounds.size() == 0);
}
