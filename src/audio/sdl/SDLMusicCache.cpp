#include "SDLMusicCache.h"

#include <iostream>

#include "SDL.h"
#include "SDL_mixer.h"

#include "audio/Music.h"

milk::MusicData* milk::SDLMusicCache::load(const char* path)
{
	std::unordered_map<std::string, MusicData*>::iterator loaded = m_music.find(path);
	if (loaded != m_music.end()) {
		++loaded->second->refCount;
		return loaded->second;
	}

	Mix_Music* music = Mix_LoadMUS(path);
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

void milk::SDLMusicCache::dereference(MusicData* musicData)
{
	if (--musicData->refCount <= 0) {
		m_music.erase(musicData->path);
		Mix_FreeMusic((Mix_Music*)musicData->handle);
		delete musicData;
	}
}

void milk::SDLMusicCache::free()
{
	for (auto itr : m_music) {
		Mix_FreeMusic((Mix_Music*)itr.second->handle);
		delete itr.second;
	}
	m_music.clear();
}
