#pragma once

#include "EventListener.h"
#include <vector>

class EventManager {
    public:
        void addListener(EventListener& listener) { listeners.push_back(&listener); }
        void handleEvent(SDL_Event& event);
    private:
        std::vector<EventListener*> listeners;

};