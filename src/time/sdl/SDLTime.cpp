#include "SDLTime.h"

#include <algorithm>

milk::SDLTime::SDLTime()
	: m_frames{ 0 }
	, m_targetFps(60.0)
	, m_fps{ 0.0 }
	, m_scale{ 0.5 }
	, m_frameStartTime{ 0.0 }
	, m_frameTime{ 0.0 }
	, m_accumulatedFrameTime{ 0.0 }
{
}

void milk::SDLTime::start()
{
	m_gameTimer.start();
	m_fpsTimer.start();
}

void milk::SDLTime::beginFrame()
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

bool milk::SDLTime::overTargetTickRate() const
{
	return m_accumulatedFrameTime >= secondsPerTick();
}

void milk::SDLTime::endFrame()
{
	m_fps = m_frames++ / m_fpsTimer.getTime();
	m_accumulatedFrameTime -= secondsPerTick();
}

double milk::SDLTime::secondsPerTick() const
{
	return 1.0 / m_targetFps;
}

void milk::SDLTime::resetFpsTimer()
{
	m_frames = 0;
	m_fpsTimer.start();
}

double milk::SDLTime::total() const
{
	return m_gameTimer.getTime();
}

double milk::SDLTime::targetFps() const
{
	return m_targetFps;
}

void milk::SDLTime::targetFps(double target)
{
	m_targetFps = std::min(std::max(target, 10.0), 60.0);
	resetFpsTimer();
}

double milk::SDLTime::fps() const
{
	return m_fps;
}

double milk::SDLTime::delta() const
{
	return secondsPerTick() * (m_scale * 2.0);
}

double milk::SDLTime::scale() const
{
	return m_scale;
}

void milk::SDLTime::scale(double sc)
{
	m_scale = std::min(std::max(sc, 0.0), 1.0);
}
