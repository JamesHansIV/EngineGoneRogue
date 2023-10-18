#include <SDL2/SDL.h>

class EventListener {
    public:
        virtual void OnEvent(SDL_Event event) = 0;
};