#pragma once

#include "Engine/Renderer/DrawElement.h"
#include "Engine/UI/Button.h"
#include "Engine/utils/utils.h"

const std::string kDefaultStartTextureID = "start-background";

class StartScreen {
   public:
    StartScreen();

    ~StartScreen();
    void Draw();
    void Update();
    State* HandleEvent(Event* event);

   private:
    TilePos m_background_src;
    DrawElement m_background;

    SDL_Rect m_title_rect;
    Button m_start_button;
};
