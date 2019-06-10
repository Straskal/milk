#include "SDLKeyboard.h"

#include <iostream>
#include <cstring>

milk::SDLKeyboard::SDLKeyboard() {
	std::memset(m_previousState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(m_currentState, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

void milk::SDLKeyboard::updateState() {
	std::memcpy(m_previousState, m_currentState, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(m_currentState, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

bool milk::SDLKeyboard::isKeyDown(SDL_Keycode keycode) {
	return m_currentState[SDL_GetScancodeFromKey(keycode)];
}

bool milk::SDLKeyboard::isKeyPressed(SDL_Keycode keycode) {
	return m_currentState[SDL_GetScancodeFromKey(keycode)] && !m_previousState[SDL_GetScancodeFromKey(keycode)];
}

bool milk::SDLKeyboard::isKeyReleased(SDL_Keycode keycode) {
	return !m_currentState[SDL_GetScancodeFromKey(keycode)] && m_previousState[SDL_GetScancodeFromKey(keycode)];
}
