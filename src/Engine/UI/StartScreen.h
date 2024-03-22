#include "Engine/UI/Button.h"

class StartScreen {
   public:
    StartScreen();
    void Draw();
    void Update();

   private:
    Texture* m_title;
    SDL_Rect m_titleRect;
    Button m_startButton;
};
