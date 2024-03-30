#pragma once

#include "Engine/Renderer/DrawElement.h"
#include "Engine/UI/Button.h"

class SettingsScreen {
   public:
    SettingsScreen();

    void Draw();
    void Update();
    void HandleEvent(Event* event);

   private:
    TilePos m_background_src;
    DrawElement m_background;
    Button m_button;
};
