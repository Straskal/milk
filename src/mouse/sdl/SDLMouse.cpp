#include "SDLMouse.h"

milk::SDLMouse::SDLMouse() {
	m_currentState = SDL_GetMouseState(&m_x, &m_y);
	m_previousState = m_currentState;
}

void milk::SDLMouse::updateState() {
	m_previousState = m_currentState;
	m_currentState = SDL_GetMouseState(&m_x, &m_y);
}

std::tuple<int, int> milk::SDLMouse::getPosition() const {
	return std::make_tuple(m_x, m_y);
}

bool milk::SDLMouse::isLeftButtonPressed() const {
	return m_currentState & SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool milk::SDLMouse::isRightButtonPressed() const {
	return m_currentState & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

bool milk::SDLMouse::wasLeftButtonPressed() const {
	return (m_currentState & SDL_BUTTON(SDL_BUTTON_LEFT) && !(m_previousState & SDL_BUTTON(SDL_BUTTON_LEFT)));
}

bool milk::SDLMouse::wasRightButtonPressed() const {
	return (m_currentState & SDL_BUTTON(SDL_BUTTON_RIGHT) && !(m_previousState & SDL_BUTTON(SDL_BUTTON_RIGHT)));
}

bool milk::SDLMouse::wasLeftButtonReleased() const {
	return !(m_currentState & SDL_BUTTON(SDL_BUTTON_LEFT) && (m_previousState & SDL_BUTTON(SDL_BUTTON_LEFT)));
}

bool milk::SDLMouse::wasRightButtonReleased() const {
	return !(m_currentState & SDL_BUTTON(SDL_BUTTON_RIGHT) && (m_previousState & SDL_BUTTON(SDL_BUTTON_RIGHT)));
}
