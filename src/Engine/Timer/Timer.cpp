#include "Timer.h"
#include "SDL2/SDL_timer.h"

Timer timer = Timer();
Timer game_timer = Timer();

Timer::Timer() {
    m_start_time = 0;
    m_paused_time = 0;
    m_is_paused = false;
    m_is_started = false;
}

void Timer::Start() {
    m_is_started = true;
    m_is_paused = false;
    m_start_time = SDL_GetTicks();
    m_current_time = m_start_time;
    m_paused_time = 0;
}

void Timer::Stop() {
    m_is_started = false;
    m_is_paused = false;
    m_start_time = 0;
    m_paused_time = 0;
}

void Timer::Pause() {
    if (m_is_started && !m_is_paused) {
        m_is_paused = true;
        m_paused_time = SDL_GetTicks() - m_start_time;
        m_start_time = 0;
    }
}

void Timer::Unpause() {
    if (m_is_paused) {
        m_is_paused = false;
        m_start_time = SDL_GetTicks() - m_paused_time;
        m_should_reset_current_time = true;
        m_paused_time = 0;
    }
}

Uint32 Timer::GetTicks() const {
    Uint32 time = 0;

    if (m_is_paused) {
        time = m_paused_time;
    } else {
        time = SDL_GetTicks() - m_start_time;
    }

    return time;
}

Uint32 Timer::GetAbsoluteTicks() {
    Uint32 time = 0;

    if (m_is_paused) {
        time = SDL_GetTicks();
    } else {
        time = SDL_GetTicks() - m_start_time;
    }

    if (m_should_reset_current_time) {
        m_current_time = time - 10;
        m_should_reset_current_time = false;
    }

    return time;
}
