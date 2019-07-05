#include "SDLTimer.h"

milk::SDLTimer::SDLTimer()
        : m_startTime{0.0}
        , m_pausedTime{0.0}
        , m_started{false}
        , m_paused{false}
{
}

void milk::SDLTimer::start()
{
    m_started = true;
    m_paused = false;
    m_startTime = SDL_GetTicks() / 1000.0;
    m_pausedTime = 0.0;
}

void milk::SDLTimer::stop()
{
    m_started = false;
    m_paused = false;
    m_startTime = 0.0;
    m_pausedTime = 0.0;
}

void milk::SDLTimer::pause()
{
    if (m_started && !m_paused) {
        m_paused = true;
        m_pausedTime = (SDL_GetTicks() / 1000.0) - m_startTime;
        m_startTime = 0.0;
    }
}

void milk::SDLTimer::resume()
{
    if (m_started && m_paused) {
        m_paused = false;
        m_startTime = (SDL_GetTicks() / 1000.0) - m_pausedTime;
        m_pausedTime = 0.0;
    }
}

double milk::SDLTimer::getTime() const
{
    double time = 0.0;
    if (m_started) {
        if (m_paused) {
            time = m_pausedTime;
        } else {
            time = (SDL_GetTicks() / 1000.0) - m_startTime;
        }
    }

    return time;
}

bool milk::SDLTimer::started() const
{
    return m_started;
}

bool milk::SDLTimer::paused() const
{
    return m_paused;
}