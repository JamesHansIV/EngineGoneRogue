#include "GameState.h"
#include "Apps/Game.h"
#include "SDL2_mixer/SDL_mixer.h"

void RunningState::Enter() {
    timer.Unpause();
    Renderer::GetInstance()->SetCameraTarget(GetGame()->GetPlayer());
    Application::Get()->GetAudioManager().SetMusicVolume(100);
    Application::Get()->GetAudioManager().PlayMusicOverride(
        "background-intense", true);
}

void RunningState::Exit() {}

void RunningState::Draw() {
    GetGame()->DrawObjects();
}

State* RunningState::Update(float dt) {
    GetGame()->UpdateObjects(dt);
    return nullptr;
}

State* RunningState::HandleEvent(Event* /*event*/) {
    return nullptr;
}

void StartState::Enter() {
    Renderer::GetInstance()->SetCameraTarget(nullptr);
    Application::Get()->GetAudioManager().SetMusicVolume(80);
    Application::Get()->GetAudioManager().PlayMusic("title-screen", true);
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

void GameOverState::Enter() {
    timer.Pause();
}

void GameOverState::Exit() {}

void GameOverState::Draw() {
    GetGame()->DrawObjects();
}

State* GameOverState::Update(float dt) {
    GetGame()->UpdateObjects(dt);
    return nullptr;
}

State* GameOverState::HandleEvent(Event* /*event*/) {
    return nullptr;
}

void PauseState::Enter() {
    timer.Pause();
    Mix_VolumeMusic(80);
    Application::Get()->GetAudioManager().PlayMusicOverride("title-screen",
                                                            true);
}

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

void LevelUpState::Enter() {}

void LevelUpState::Exit() {}

void LevelUpState::Draw() {
    GetGame()->DrawObjects();
    m_option_one_button.Draw();
    m_option_two_button.Draw();
    m_option_three_button.Draw();
}

State* LevelUpState::Update(float dt) {
    GetGame()->UpdateObjects(dt);
    m_option_one_button.Update();
    m_option_two_button.Update();
    m_option_three_button.Update();
    return nullptr;
}

State* LevelUpState::HandleEvent(Event* /*event*/) {
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

State* ShopState::HandleEvent(Event* /*event*/) {
    return nullptr;
}
