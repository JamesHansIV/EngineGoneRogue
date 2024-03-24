#include "StartScreen.h"

StartScreen::StartScreen() : m_title(nullptr) {
    m_title = Renderer::GetInstance()->AddTextTexture(
        "GameTitle", "Rogue Mansion", {255, 255, 255, 255});
    const int window_width = Application::Get()->GetWindowWidth();
    const int window_height = Application::Get()->GetWindowHeight();

    m_titleRect = {60, 50, window_width - 150, window_height / 6};

    int const x = (window_width - 100) / 2;
    int const y = (window_height - 60) / 2;
    m_startButton = Button(SDL_Rect{x, y, 100, 60}, "Start", []() {
        SDL_Log("Start button clicked");
        timer.Unpause();
        PushNewEvent(EventType::StartGameEvent);
    });
}

void StartScreen::Draw() {
    SDL_Rect src = {0, 0, m_title->GetWidth(), m_title->GetHeight()};
    Renderer::GetInstance()->Draw("GameTitle", src, m_titleRect, SDL_FLIP_NONE);
    m_startButton.Draw();
}

void StartScreen::Update() {
    m_startButton.Update();
}
