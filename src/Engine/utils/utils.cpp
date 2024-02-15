#include "utils.h"
#include "Engine/Events/Event.h"
#include "SDL2/SDL_events.h"

void PushNewEvent(EventType eventType, void* data1, void* data2) {
    SDL_Event sdl_event;
    sdl_event.type = custom_event_type;
    sdl_event.user.code = static_cast<int>(eventType);
    sdl_event.user.data1 = data1;
    sdl_event.user.data2 = data2;
    SDL_PushEvent(&sdl_event);
}
