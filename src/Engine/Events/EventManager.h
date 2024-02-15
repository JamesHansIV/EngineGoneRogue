#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/GameObject.h"

class EventManager {
   public:
    // Init custom events in event Manager
    EventManager() = default;
    EventManager(const EventManager&) = default;
    EventManager(EventManager&&) = default;
    EventManager& operator=(const EventManager&) = delete;
    EventManager& operator=(EventManager&&) = delete;
    // Handle custom events and SDL events
    virtual void HandleEvents();

   protected:
};
