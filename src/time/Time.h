#ifndef _TIME_H_
#define _TIME_H_

#include <algorithm>

#include "Timer.h"

namespace milk
{
	class Time
	{
	public:
		Time() 
			: m_targetFps(60.0)
			, m_fps{ 0.0 }
			, m_frames{ 0 }
			, m_scale{ 0.5f }
			, m_frameStartTime{ 0.0 }
			, m_frameTime{ 0.0 }
			, m_accumulatedFrameTime{ 0.0 }
		{
		}

		inline void start() 
		{
			m_gameTimer.start();
			m_fpsTimer.start();
		}

		inline double total() const 
		{
			return m_gameTimer.getTime();
		}

		inline double targetFps() const
		{
			return m_targetFps;
		}

		inline void targetFps(const double target)
		{
			m_targetFps = std::min(std::max(target, 10.0), 60.0);
			resetFpsTimer();
		}

		inline double fps() const
		{
			return m_fps;
		}

		inline double delta() const 
		{
			return secondsPerTick() * (m_scale * 2.0);
		}

		inline float scale() const
		{
			return m_scale;
		}

		inline void scale(const float v)
		{
			m_scale = std::min(std::max(v, 0.f), 1.f);
		}

		inline double secondsPerTick() const 
		{
			return 1.0 / m_targetFps;
		}

		inline void resetFpsTimer() 
		{
			m_frames = 0;
			m_fpsTimer.start();
		}

		inline void beginFrame() 
		{
			const double t = total();
			m_frameTime = t - m_frameStartTime;
			m_frameStartTime = t;

			// If we hit a breakpoint, then we don't want the next frame to be insane in the membrane.
			if (m_frameTime > 1.0) {
				m_frameTime = secondsPerTick();
				resetFpsTimer();
			}

			m_accumulatedFrameTime += m_frameTime;
		}

		inline bool overTargetTickRate() const 
		{
			return m_accumulatedFrameTime >= secondsPerTick();
		}

		inline void endFrame() 
		{
			m_fps = m_frames++ / m_fpsTimer.getTime();
			m_accumulatedFrameTime -= secondsPerTick();
		}

	private:
		double m_targetFps;
		double m_fps;
		int m_frames;
		float m_scale;

		double m_frameStartTime;
		double m_frameTime;
		double m_accumulatedFrameTime;

		Timer m_gameTimer;
		Timer m_fpsTimer;
	};
}

#endif