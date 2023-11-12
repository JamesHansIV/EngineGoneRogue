#pragma once

#include "EventListener.h"
#include <vector>

class EventManager {
    public:
        void AddListener(EventListener& listener) { m_listeners.push_back(&listener); }
        void HandleEvent(SDL_Event& event);
    private:
        std::vector<EventListener*> m_listeners;

};