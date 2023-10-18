#include "EventHandler.h"
#include "Event.h"

void EventHandler::handleEvent(SDL_Event& event) {
    Event evt(event);
    for (const EventListener& listener : listeners) {
        listener.OnEvent(evt);
    }
}