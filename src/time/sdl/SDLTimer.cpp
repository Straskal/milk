#include "SDLTimer.h"

milk::SDLTimer::SDLTimer()
		: m_startTime{0.0}
		, m_started{false}
{
}

void milk::SDLTimer::start()
{
	m_started = true;
	m_startTime = SDL_GetTicks() / 1000.0;
}

double milk::SDLTimer::getTime() const
{
	double time = 0.0;
	if (m_started) {
		time = (SDL_GetTicks() / 1000.0) - m_startTime;
	}
	return time;
}