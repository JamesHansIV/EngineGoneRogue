#include "GameState.h"
#include "Apps/Game.h"

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
    return nullptr;
}

void StartState::Enter() {
    Renderer::GetInstance()->SetCameraTarget(nullptr);
    timer.Pause();
}

void StartState::Exit() {}

void StartState::Draw() {
    GetGame()->DrawObjects();
    m_button.Draw();
}

State* StartState::Update(float dt) {
    GetGame()->UpdateObjects(dt);
    m_button.Update();
    return nullptr;
}

State* StartState::HandleEvent(Event* event) {
    switch (event->GetEventType()) {
        case EventType::StartGameEvent: {
            return new RunningState(GetGame());
        }
        default:
            break;
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

void PauseState::Enter() {}

void PauseState::Exit() {}

void PauseState::Draw() {
    GetGame()->DrawObjects();
    m_button.Draw();
}

State* PauseState::Update(float dt) {
    m_button.Update();
    GetGame()->UpdateObjects(dt);
    return nullptr;
}

State* PauseState::HandleEvent(Event* event) {
    switch (event->GetEventType()) {
        case EventType::ContinueGameEvent:
            return new RunningState(GetGame());
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
