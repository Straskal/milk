#include "Timer.h"

#include "SDL.h"

milk::Timer::Timer()
        : startTicks_(0),
          pausedTicks_(0),
          paused_(false),
          started_(false)
{
}

void milk::Timer::start()
{
    started_ = true;
    paused_ = false;
    startTicks_ = SDL_GetTicks();
    pausedTicks_ = 0;
}

void milk::Timer::stop()
{
    started_ = false;
    paused_ = false;
    startTicks_ = 0;
    pausedTicks_ = 0;
}

void milk::Timer::pause()
{
    if (started_ && !paused_)
    {
        paused_ = true;
        pausedTicks_ = SDL_GetTicks() - startTicks_;
        startTicks_ = 0;
    }
}

void milk::Timer::unpause()
{
    if (started_ && paused_)
    {
        paused_ = false;
        startTicks_ = SDL_GetTicks() - pausedTicks_;
        pausedTicks_ = 0;
    }
}

int milk::Timer::milliseconds()
{
    Uint32 time = 0;

    if (started_)
    {
        if (paused_)
            time = (Uint32)pausedTicks_;
        else
            time = SDL_GetTicks() - startTicks_;

    }

    return time;
}

float milk::Timer::seconds()
{
    return milliseconds() / 1000.f;
}

bool milk::Timer::isStarted()
{
    return started_;
}

bool milk::Timer::isPaused()
{
    return paused_ && started_;
}
