#ifndef _SDL_TIMER_H_
#define _SDL_TIMER_H_

#include "SDL.h"

namespace milk 
{
	class SDLTimer
	{
	public:
		SDLTimer();

		void start();
		double getTime() const;

	private:
		double m_startTime;
		bool m_started;
	};
}

#endif