#include "SDLTime.h"

#include <algorithm>

#include "SDL.h"

milk::SDLTime::SDLTime()
	: m_startTime{ 0.0 }
{
}

void milk::SDLTime::start()
{
	m_startTime = SDL_GetTicks() / 1000.0;
}
double milk::SDLTime::total() const
{
	return (SDL_GetTicks() / 1000.0) - m_startTime;
}

void milk::SDLTime::delay(double seconds)
{
	SDL_Delay((Uint32)(seconds * 1000));
}
