#include "EventManager.h"

void EventManager::handleEvent(SDL_Event& event) {
    Event evt(event);
    for (EventListener* listener : listeners) {
        listener->OnEvent(evt);
    }
}