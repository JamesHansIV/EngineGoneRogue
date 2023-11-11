#include "EventManager.h"

void EventManager::HandleEvent(SDL_Event& event) {
    Event evt(event);
    for (EventListener* listener : m_listeners) {
        listener->OnEvent(evt);
    }
}