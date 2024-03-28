#pragma once

#include "Engine/Renderer/DrawElement.h"
#include "Engine/UI/Button.h"

const std::string kDefaultGameOverScreenTextureID = "game-over-background";

class GameOverScreen {
   public:
    GameOverScreen();

    void Draw();
    void Update();
    State* HandleEvent(Event* event);

   private:
    DrawElement m_background;
    TilePos m_background_src;
    Button m_restart;
};
