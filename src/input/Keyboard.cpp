#include "Keyboard.h"

#include <iostream>
#include <cstring>

Uint8 milk::Keyboard::previousState_[SDL_NUM_SCANCODES];
Uint8 milk::Keyboard::currentState_[SDL_NUM_SCANCODES];

bool milk::Keyboard::getKey(SDL_Keycode keycode) {
	return currentState_[SDL_GetScancodeFromKey(keycode)];
}

bool milk::Keyboard::getKeyPressed(SDL_Keycode keycode) {
	return currentState_[SDL_GetScancodeFromKey(keycode)] && !previousState_[SDL_GetScancodeFromKey(keycode)];
}

bool milk::Keyboard::getKeyReleased(SDL_Keycode keycode) {
	return !currentState_[SDL_GetScancodeFromKey(keycode)] && previousState_[SDL_GetScancodeFromKey(keycode)];
}

void milk::Keyboard::initialize() {
	std::memset(previousState_, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(currentState_, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

void milk::Keyboard::updateKeyboardState() {
	std::memcpy(previousState_, currentState_, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(currentState_, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}
