#ifndef _SDL_TIME_H_
#define _SDL_TIME_H_

#include "SDLTimer.h"
#include "time/Time.h"

namespace milk
{
	class SDLTime : public Time
	{
	public:
		SDLTime();

		void start();
		void beginFrame();
		bool overTargetTickRate() const;
		void endFrame();

		double secondsPerTick() const;
		void resetFpsTimer();

		double total() const override;
		double targetFps() const override;
		void targetFps(double target) override;
		double fps() const override;
		double delta() const override;
		double scale() const override;
		void scale(double sc) override;

	private:
		int m_frames;
		double m_targetFps;
		double m_fps;
		double m_scale;

		double m_frameStartTime;
		double m_frameTime;
		double m_accumulatedFrameTime;

		SDLTimer m_gameTimer;
		SDLTimer m_fpsTimer;
	};
}

#endif