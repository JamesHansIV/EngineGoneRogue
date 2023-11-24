#pragma once

#include <vector>
#include "EventListener.h"

class EventManager {
   public:
    void AddListener(EventListener& listener) {
        m_listeners.push_back(&listener);
    }

    void HandleEvent(SDL_Event& event);

   private:
    std::vector<EventListener*> m_listeners;
};