#pragma once

#include "Event.h"

class EventListener {
   public:
    virtual void OnEvent(Event& event) = 0;
};