#include "SDLTextureCache.h"

#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

#include "video/Texture.h"

milk::SDLTextureCache::SDLTextureCache()
	: m_rendererHandle{ 0 } {
}

bool milk::SDLTextureCache::init(SDL_Renderer* rendererHandle) {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags) {
		std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
		return false;
	}
	m_rendererHandle = rendererHandle;
	return true;
}

milk::TextureData* milk::SDLTextureCache::load(const std::string& path) {
	auto found = m_textures.find(path);
	if (found != m_textures.end()) {
		++found->second->refCount;
		return found->second;
	}

	SDL_Surface* sdlsurface = IMG_Load(path.c_str());
	if (sdlsurface == nullptr) {
		std::cout << "Error loading image: " << IMG_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* sdltexture = SDL_CreateTextureFromSurface(m_rendererHandle, sdlsurface);
	SDL_FreeSurface(sdlsurface);

	int w, h;
	SDL_QueryTexture(sdltexture, nullptr, nullptr, &w, &h);

	TextureData* textureData = new TextureData();
	textureData->path = path;
	textureData->refCount = 1;
	textureData->handle = sdltexture;
	textureData->width = w;
	textureData->height = h;

	m_textures.insert(std::make_pair(path, textureData));
	return textureData;
}

void milk::SDLTextureCache::dereference(TextureData* textureData) {
	if (--textureData->refCount <= 0) {
		m_textures.erase(textureData->path);
		SDL_DestroyTexture((SDL_Texture*)textureData->handle);
		delete textureData; textureData = nullptr;
	}
}

void milk::SDLTextureCache::free() {
	SDL_assert(m_textures.size() == 0);
	IMG_Quit();
}
