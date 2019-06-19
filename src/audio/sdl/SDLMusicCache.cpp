#include "SDLMusicCache.h"

#include <iostream>

#include "SDL.h"
#include "SDL_mixer.h"

#include "audio/Music.h"

milk::MusicData* milk::SDLMusicCache::load(const std::string& path) {
	auto found = m_music.find(path);
	if (found != m_music.end()) {
		++found->second->refCount;
		return found->second;
	}

	Mix_Music* music = Mix_LoadMUS(path.c_str());
	if (!music) {
		std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
		return nullptr;
	}

	MusicData* musicData = new MusicData();
	musicData->path = path;
	musicData->handle = music;
	musicData->refCount = 1;

	m_music.insert(std::make_pair(path, musicData));
	return musicData;
}

void milk::SDLMusicCache::dereference(MusicData* musicData) {
	if (--musicData->refCount <= 0) {
		m_music.erase(musicData->path);
		Mix_FreeMusic((Mix_Music*)musicData->handle);
		delete musicData; musicData = nullptr;
	}
}

void milk::SDLMusicCache::free() {
	SDL_assert(m_music.size() == 0);
}
