#include "SDLMouse.h"

milk::SDLMouse::SDLMouse()
	: m_currentState{ 0 }
	, m_previousState{ 0 }
	, m_x{ 0 }
	, m_y{ 0 }
	, m_scroll{ 0 } {}

void milk::SDLMouse::frameBegin()
{
	m_scroll = 0;
}

void milk::SDLMouse::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_MOUSEWHEEL) {
		m_scroll = e->wheel.y;
	}
}

void milk::SDLMouse::updateState()
{
	m_previousState = m_currentState;
	m_currentState = SDL_GetMouseState(&m_x, &m_y);
}

std::tuple<int, int> milk::SDLMouse::getPosition() const
{
	return std::make_tuple(m_x, m_y);
}

bool milk::SDLMouse::isButtonDown(MouseButtons button) const
{
	return m_currentState & SDL_BUTTON((int)button);
}

bool milk::SDLMouse::isButtonPressed(MouseButtons button) const
{
	return (m_currentState & SDL_BUTTON((int)button) && !(m_previousState & SDL_BUTTON((int)button)));
}

bool milk::SDLMouse::isButtonReleased(MouseButtons button) const
{
	return !(m_currentState & SDL_BUTTON((int)button) && (m_previousState & SDL_BUTTON((int)button)));
}

int milk::SDLMouse::getScroll() const
{
	return m_scroll;
}
