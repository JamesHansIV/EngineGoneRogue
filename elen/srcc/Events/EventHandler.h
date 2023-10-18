#include <SDL2/SDL.h>

#include "Event"

class EventHandler {
    public:
        EventHandler();
        ~EventHandler();
        
        void addListener(EventListener& listener) { listeners.push_back(listener); }
        void handleEvent(SDL_Event& event);
    private:
        std::vector<EventListener> listeners;

};