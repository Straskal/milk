#include "Texture.h"

#include"SDL.h"

milk::Texture::Texture(SDL_Texture* sdlTexture, int width, int height)
	: sdlTexture_(sdlTexture),
	width_(width),
	height_(height) {
}

milk::Texture::~Texture() {
	SDL_DestroyTexture(sdlTexture_);
}

SDL_Texture* milk::Texture::get() const {
	return sdlTexture_;
}

int milk::Texture::width() const {
	return width_;
}

int milk::Texture::height() const {
	return height_;
}
