#include "time.h"

#include "SDL.h"

double milk::time_get_total()
{
	return SDL_GetTicks() / 1000.0;
}

void milk::time_delay(double seconds)
{
	SDL_Delay((Uint32)(seconds * 1000));
}
