#include "SDLTextureCache.h"

#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

#include "video/Texture.h"

milk::SDLTextureCache::SDLTextureCache()
	: m_rendererHandle{ 0 } {
}

bool milk::SDLTextureCache::init(SDL_Renderer* rendererHandle) {
	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
		std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
		return false;
	}
	m_rendererHandle = rendererHandle;
	return true;
}

milk::Texture* milk::SDLTextureCache::reference(const std::string& path) {
	auto found = m_textures.find(path);
	if (found != m_textures.end()) {
		return found->second;
	}

	SDL_Surface* sdlsurface = IMG_Load(path.c_str());
	if (sdlsurface == nullptr) {
		std::cout << "Error loading image: " << IMG_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* sdltexture = SDL_CreateTextureFromSurface(m_rendererHandle, sdlsurface);
	SDL_FreeSurface(sdlsurface);

	int width;
	int height;
	SDL_QueryTexture(sdltexture, nullptr, nullptr, &width, &height);

	Texture* texture = new Texture();
	texture->path = path;
	texture->refCount = 1;
	texture->handle = sdltexture;
	texture->width = width;
	texture->height = height;

	m_textures.insert(std::make_pair(path, texture));
}

void milk::SDLTextureCache::dereference(Texture* texture) {
	if (--texture->refCount <= 0) {
		m_textures.erase(texture->path);
		SDL_DestroyTexture((SDL_Texture*)texture->handle);
		delete texture; texture = nullptr;
	}
}

void milk::SDLTextureCache::free() {
	IMG_Quit();
}
