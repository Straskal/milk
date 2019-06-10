#include "Texture.h"

#include"SDL.h"

milk::Texture::Texture(SDL_Texture* sdlTexture, int width, int height)
	: m_handle(sdlTexture),
	m_width(width),
	m_height(height) {
}

milk::Texture::~Texture() {
	SDL_DestroyTexture(m_handle);
}

SDL_Texture* milk::Texture::get() const {
	return m_handle;
}

int milk::Texture::width() const {
	return m_width;
}

int milk::Texture::height() const {
	return m_height;
}
