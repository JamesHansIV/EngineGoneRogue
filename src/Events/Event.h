#include <SDL2/SDL.h>

class Event {
    public:
        bool handled;
    public:
        Event(SDL_Event& event) : handled(false), m_Event(event) {}
        ~Event() {}
        SDL_Event& getEvent() { return m_Event; }
    private:
        SDL_Event& m_Event;
};