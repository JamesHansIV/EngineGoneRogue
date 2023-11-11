#pragma once

#include <SDL2/SDL.h>

class Event {
    public:
        bool handled{false};
    
        explicit Event(SDL_Event& event) :  m_Event(event) {}
        ~Event() = default;
        SDL_Event& GetEvent() { return m_Event; }
    private:
        SDL_Event& m_Event;
};