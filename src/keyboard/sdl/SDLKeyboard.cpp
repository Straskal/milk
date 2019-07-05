#include "SDLKeyboard.h"

#include <iostream>
#include <cstring>

milk::SDLKeyboard::SDLKeyboard()
	: m_currentState{}
	, m_previousState{}
{
	std::memset(m_currentState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memset(m_previousState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
}

void milk::SDLKeyboard::tick()
{
	std::memcpy(m_previousState, m_currentState, sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(m_currentState, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

bool milk::SDLKeyboard::isKeyDown(Keys key) const
{
	return m_currentState[(SDL_Scancode)key];
}

bool milk::SDLKeyboard::isKeyPressed(Keys key) const
{
	return m_currentState[(SDL_Scancode)key] && !m_previousState[(SDL_Scancode)key];
}

bool milk::SDLKeyboard::isKeyReleased(Keys key) const
{
	return !m_currentState[(SDL_Scancode)key] && m_previousState[(SDL_Scancode)key];
}
