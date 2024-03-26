#include "PauseScreen.h"

PauseScreen::PauseScreen(const Player& player)
    : m_player_stats(player.GetStats()), m_inventory(player.GetPlayerItems()) {
    int const x = (Application::Get()->GetWindowWidth() - 100) / 2;
    int const y = (Application::Get()->GetWindowHeight() - 60) / 2;
    m_button = Button("buttons", SDL_Rect{x, y, 150, 80}, "Continue", []() {
        SDL_Log("Continue button clicked");
        timer.Unpause();
        PushNewEvent(EventType::ContinueGameEvent);
    });
}

void PauseScreen::Draw() {
    m_inventory.Draw();
    m_player_stats.Draw();
    m_button.Draw();
}

void PauseScreen::Update(const Player& player) {
    m_inventory.Update(player);
    m_player_stats.Update(player);
    m_button.Update();
}

State* PauseScreen::HandleEvent(Event* event) {
    State* state = nullptr;
    if (event->GetEventType() == EventType::MouseButtonDownEvent) {
        //m_button.HandleEvent(static_cast<MouseButtonDownEvent*>(event));
    }
    return state;
}
