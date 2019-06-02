#include "Keyboard.h"

#include <iostream>
#include <cstring>

Uint8 milk::Keyboard::m_previousState[SDL_NUM_SCANCODES];
Uint8 milk::Keyboard::m_currentState[SDL_NUM_SCANCODES];

bool milk::Keyboard::getKey(SDL_Keycode keycode) {
	return m_currentState[SDL_GetScancodeFromKey(keycode)];
}

bool milk::Keyboard::getKeyPressed(SDL_Keycode keycode) {
	return m_currentState[SDL_GetScancodeFromKey(keycode)] && !m_previousState[SDL_GetScancodeFromKey(keycode)];
}

bool milk::Keyboard::getKeyReleased(SDL_Keycode keycode) {
	return !m_currentState[SDL_GetScancodeFromKey(keycode)] && m_previousState[SDL_GetScancodeFromKey(keycode)];
}

void milk::Keyboard::initialize() {
	std::memset(m_previousState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(m_currentState, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

void milk::Keyboard::updateKeyboardState() {
	std::memcpy(m_previousState, m_currentState, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(m_currentState, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}
