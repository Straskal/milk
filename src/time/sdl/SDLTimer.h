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
		void stop();
		void pause();
		void resume();
		double getTime() const;
		bool started() const;
		bool paused() const;

	private:
		double m_startTime;
		double m_pausedTime;
		bool m_started;
		bool m_paused;
	};
}

#endif