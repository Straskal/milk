#include "SDLAudioCache.h"

#include <iostream>

#include "SDL_mixer.h"

#include "audio/Sound.h"

milk::Sound* milk::SDLAudioCache::loadSound(const std::string& path) {
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
	sound->channel = -1;
	sound->refCount = 1;
	sound->handle = sample;

	m_sounds.insert(std::make_pair(path, sound));
	return sound;
}

void milk::SDLAudioCache::dereferenceSound(Sound* sound) {
	if (--sound->refCount <= 0) {
		m_sounds.erase(sound->path);
		Mix_FreeChunk((Mix_Chunk*)sound->handle);
		delete sound; sound = nullptr;
	}
}
