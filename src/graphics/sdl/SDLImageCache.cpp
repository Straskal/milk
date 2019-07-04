#include "SDLImageCache.h"

#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

#include "graphics/Image.h"

milk::SDLImageCache::SDLImageCache()
	: m_rendererHandle{ nullptr }
{
}

bool milk::SDLImageCache::init(SDL_Renderer* rendererHandle)
{
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags) {
		std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
		return false;
	}
	m_rendererHandle = rendererHandle;
	return true;
}

milk::ImageData* milk::SDLImageCache::load(const char* path)
{
	std::unordered_map<std::string, ImageData*>::iterator loaded = m_images.find(path);
	if (loaded != m_images.end()) {
		++loaded->second->refCount;
		return loaded->second;
	}

	SDL_Surface* sdlsurface = IMG_Load(path);
	if (sdlsurface == nullptr) {
		std::cout << "Error loading image: " << IMG_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* sdltexture = SDL_CreateTextureFromSurface(m_rendererHandle, sdlsurface);
	SDL_FreeSurface(sdlsurface);

	int w, h;
	SDL_QueryTexture(sdltexture, nullptr, nullptr, &w, &h);

	ImageData* imageData = new ImageData();
	imageData->path = path;
	imageData->refCount = 1;
	imageData->handle = sdltexture;
	imageData->width = w;
	imageData->height = h;

	m_images.insert(std::make_pair(path, imageData));
	return imageData;
}

void milk::SDLImageCache::dereference(ImageData* imageData)
{
	if (--imageData->refCount <= 0) {
		m_images.erase(imageData->path);
		SDL_DestroyTexture((SDL_Texture*)imageData->handle);
		delete imageData; 
		imageData = nullptr;
	}
}

void milk::SDLImageCache::free()
{
	SDL_assert(m_images.size() == 0);
	IMG_Quit();
}
