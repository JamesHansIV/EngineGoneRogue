#include "GameState.h"
#include "Apps/Game.h"
#include "Engine/Events/Event.h"
#include "Engine/State/State.h"
#include "SDL2/SDL_keycode.h"

void RunningState::Enter() {
    timer.Unpause();
    Renderer::GetInstance()->SetCameraTarget(GetGame()->GetPlayer());
}

void RunningState::Exit() {}

void RunningState::Draw() {
    GetGame()->DrawObjects();
}

State* RunningState::Update(float dt) {
    GetGame()->UpdateObjects(dt);
    return nullptr;
}

State* RunningState::HandleEvent(Event* event) {
    switch (event->GetEventType()) {
        case EventType::ButtonDownEvent:
            switch (dynamic_cast<ButtonDownEvent*>(event)->GetButton()) {
                case SDLK_ESCAPE:
                    return new PauseState(GetGame());
                default:
                    break;
            }
            break;
        case EventType::PlayerLevelUpEvent:
            return new PauseState(GetGame());
            break;
        default:
            break;
    }
    return nullptr;
}

void StartState::Enter() {
    Renderer::GetInstance()->SetCameraTarget(nullptr);
    m_ButtonW = 100;
    m_ButtonH = 60;
    m_ButtonX = (Application::Get()->GetWindowWidth() - m_ButtonW) / 2;
    m_ButtonY = (Application::Get()->GetWindowHeight() - m_ButtonH) / 2;
}

void StartState::Exit() {}

void StartState::Draw() {
    SDL_Rect src = {0, 0, m_ButtonW, m_ButtonH};
    SDL_Rect dst = {m_ButtonX, m_ButtonY, m_ButtonW, m_ButtonH};

    Renderer::GetInstance()->AddTextTexture("start_button", "Start",
                                            {255, 255, 255, 255});

    Renderer::GetInstance()->DrawRect(dst, {255, 255, 255, 255});
    Renderer::GetInstance()->Draw("start_button", src, dst);
}

State* StartState::Update(float dt) {
    //GetGame()->UpdateObjects(dt);
    return nullptr;
}

State* StartState::HandleEvent(Event* event) {
    switch (event->GetEventType()) {
        case EventType::MouseDownEvent:
            auto* e = dynamic_cast<MouseDownEvent*>(event);
            if (e->GetButton() == SDL_BUTTON_LEFT) {
                if (m_ButtonX <= e->GetX() &&
                    e->GetX() <= m_ButtonX + m_ButtonW &&
                    m_ButtonY <= e->GetY() &&
                    e->GetY() <= m_ButtonY + m_ButtonH) {
                    return new RunningState(GetGame());
                }
            }
    }
    return nullptr;
}

void GameOverState::Enter() {}

void GameOverState::Exit() {}

void GameOverState::Draw() {
    GetGame()->DrawObjects();
}

State* GameOverState::Update(float dt) {
    GetGame()->UpdateObjects(dt);
    return nullptr;
}

State* GameOverState::HandleEvent(Event* event) {
    return nullptr;
}

void PauseState::Enter() {
    timer.Pause();
}

void PauseState::Exit() {
}

void PauseState::Draw() {
    GetGame()->DrawObjects();
}

State* PauseState::Update(float dt) {
    return nullptr;
}

State* PauseState::HandleEvent(Event* event) {
    switch (event->GetEventType()) {
        case EventType::ButtonDownEvent:
            switch (dynamic_cast<ButtonDownEvent*>(event)->GetButton()) {
                case SDLK_ESCAPE:
                    return new RunningState(GetGame());
                default:
                    break;
            }
        default:
            break;
    }
    return nullptr;
}

void ShopState::Enter() {}

void ShopState::Exit() {}

void ShopState::Draw() {
    GetGame()->DrawObjects();
}

State* ShopState::Update(float dt) {
    GetGame()->UpdateObjects(dt);
    return nullptr;
}

State* ShopState::HandleEvent(Event* event) {
    return nullptr;
}
