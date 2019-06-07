#include "Keyboard.h"

#include <iostream>
#include <cstring>

bool milk::sdl::Keyboard::isKeyDown(SDL_Keycode keycode) {
	return m_currentState[SDL_GetScancodeFromKey(keycode)];
}

bool milk::sdl::Keyboard::isKeyPressed(SDL_Keycode keycode) {
	return m_currentState[SDL_GetScancodeFromKey(keycode)] && !m_previousState[SDL_GetScancodeFromKey(keycode)];
}

bool milk::sdl::Keyboard::isKeyReleased(SDL_Keycode keycode) {
	return !m_currentState[SDL_GetScancodeFromKey(keycode)] && m_previousState[SDL_GetScancodeFromKey(keycode)];
}

void milk::sdl::Keyboard::init() {
	std::memset(m_previousState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(m_currentState, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

void milk::sdl::Keyboard::updateState() {
	std::memcpy(m_previousState, m_currentState, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(m_currentState, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}
