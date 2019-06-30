#ifndef _TIMER_H_
#define _TIMER_H_

#include "SDL.h"

namespace milk 
{
	class Timer
	{
	public:
		Timer() 
			: m_startTime{ 0.0 }
			, m_pausedTime{ 0.0 }
			, m_started{ false }
			, m_paused{ false }
		{
		}

		inline void start() 
		{
			m_started = true;
			m_paused = false;
			m_startTime = SDL_GetTicks() / 1000.0;
			m_pausedTime = 0.0;
		}

		inline void stop() 
		{
			m_started = false;
			m_paused = false;
			m_startTime = 0.0;
			m_pausedTime = 0.0;
		}

		inline void pause() 
		{
			if (m_started && !m_paused) {
				m_paused = true;
				m_pausedTime = (SDL_GetTicks() / 1000.0) - m_startTime;
				m_startTime = 0.0;
			}
		}

		inline void resume() 
		{
			if (m_started && m_paused) {
				m_paused = false;
				m_startTime = (SDL_GetTicks() / 1000.0) - m_pausedTime;
				m_pausedTime = 0.0;
			}
		}

		inline double getTime() const
		{
			double time = 0.0;
			if (m_started) {
				if (m_paused) {
					time = m_pausedTime;
				}
				else {
					time = (SDL_GetTicks() / 1000.0) - m_startTime;
				}
			}

			return time;
		}

		inline bool started() const
		{
			return m_started;
		}

		inline bool paused() const
		{
			m_paused;
		}

	private:
		double m_startTime;
		double m_pausedTime;
		bool m_paused;
		bool m_started;
	};
}

#endif