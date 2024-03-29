#include "GameOverScreen.h"

GameOverScreen::GameOverScreen() {
    const int window_width = Application::Get().GetWindowWidth();
    const int window_height = Application::Get().GetWindowHeight();
    m_background = DrawElement(kDefaultGameOverScreenTextureID,
                               {0, 0, window_width, window_height});

    Texture* texture =
        Renderer::GetInstance().GetTexture(kDefaultGameOverScreenTextureID);

    m_background_src = {0, 0, texture->GetWidth(), texture->GetHeight()};

    int const x = (window_width - 150) / 2;
    int const y = window_height - 125;
    m_restart = Button("buttons", SDL_Rect{x, y, 150, 80}, "Start Over", []() {
        SDL_Log("Restart button clicked");
        timer.Unpause();
        PushNewEvent(EventType::RestartGameEvent);
    });
}

void GameOverScreen::Draw() {
    m_background.Draw(m_background_src);
    m_restart.Draw();
}

void GameOverScreen::Update() {
    m_restart.Update();
}

State* GameOverScreen::HandleEvent(Event* event) {
    if (event->GetEventType() == EventType::MouseButtonDownEvent) {
        //m_restart.HandleEvent(static_cast<MouseButtonDownEvent*>(event));
    }
    return nullptr;
}
