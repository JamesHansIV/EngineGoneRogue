#pragma once

#include "Engine/HeadsUpDisplay/TimerElement.h"

class HUD {
   public:
    HUD();

    void Draw() const;

   private:
    TimerElement m_TimerElement{};
};
