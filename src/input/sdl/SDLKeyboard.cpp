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

bool milk::SDLKeyboard::isKeyDown(Keys keycode) {
	return m_currentState[SDL_GetScancodeFromKey((SDL_Keycode)keycode)];
}

bool milk::SDLKeyboard::isKeyPressed(Keys keycode) {
	return m_currentState[SDL_GetScancodeFromKey((SDL_Keycode)keycode)] && !m_previousState[SDL_GetScancodeFromKey((SDL_Keycode)keycode)];
}

bool milk::SDLKeyboard::isKeyReleased(Keys keycode) {
	return !m_currentState[SDL_GetScancodeFromKey((SDL_Keycode)keycode)] && m_previousState[SDL_GetScancodeFromKey((SDL_Keycode)keycode)];
}
