#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "Engine/utils/utils.h"
#include "SDL2/SDL_timer.h"

#include <cassert>
#include <map>
#include <set>
#include <string>

class Timer {
   public:
    Timer();

    void Start();
    void Stop();
    void Pause();
    void Unpause();

    [[nodiscard]] Uint32 GetTicks() const;

    [[nodiscard]] Uint32 GetStartTime() const { return m_start_time; }

    Uint32 SetStartTime(Uint32 tick) { return m_start_time = tick; }

    [[nodiscard]] Uint32 GetPausedTime() const { return m_paused_time; }

    Uint32 SetPausedTime(Uint32 tick) { return m_paused_time = tick; }

    [[nodiscard]] Uint32 GetCurrentTime() const { return m_current_time; }

    Uint32 SetCurrentTime(Uint32 tick) { return m_current_time = tick; }

    [[nodiscard]] bool IsStarted() const { return m_is_started; }

    [[nodiscard]] bool IsPaused() const { return m_is_paused; }

   private:
    Uint32 m_start_time;
    Uint32 m_current_time;
    Uint32 m_paused_time;
    bool m_is_paused;
    bool m_is_started;
};

// Global timers
extern Timer timer;
