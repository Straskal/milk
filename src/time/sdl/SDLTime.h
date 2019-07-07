#ifndef _SDL_TIME_H_
#define _SDL_TIME_H_

#include "time/Time.h"

namespace milk
{
	class SDLTime : public Time
	{
	public:
		SDLTime();

		void start();
		double total() const override;
		void delay(double seconds) override;

	private:
		double m_startTime;
	};
}

#endif