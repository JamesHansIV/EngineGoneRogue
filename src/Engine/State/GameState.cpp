#include "GameState.h"
#include "Apps/Game.h"
#include "Engine/Objects/Projectiles/ProjectileManager.h"
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
    m_startScreen = StartScreen();
    Renderer::GetInstance()->SetCameraTarget(nullptr);
    Renderer::GetInstance()->SetCamera(0, 0);
    Application::Get()->GetAudioManager().SetMusicVolume(80);
    Application::Get()->GetAudioManager().PlayMusic("pause-screen", true);
    timer.Pause();
}

void StartState::Exit() {}

void StartState::Draw() {
    GetGame()->DrawObjects();
    m_startScreen.Draw();
}

State* StartState::Update(float dt) {
    GetGame()->UpdateObjects(dt);
    m_startScreen.Update();
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

void RoomTransitionState::Enter() {}

void RoomTransitionState::Exit() {
    GetGame()->LoadNextRoom();
}

void RoomTransitionState::Draw() {}

State* RoomTransitionState::Update(float dt) {
    if (timer.GetTicks() - m_enter_time > m_transition_time) {
        SDL_Log("in room transition state to running state: %d",
                m_transition_time);
        return new RunningState(GetGame());
    }
    return nullptr;
}

State* RoomTransitionState::HandleEvent(Event* event) {
    return nullptr;
}

void GameOverState::Enter() {
    timer.Pause();
}

void GameOverState::Exit() {}

void GameOverState::Draw() {
    m_game_over_screen.Draw();
}

State* GameOverState::Update(float dt) {
    m_game_over_screen.Update();
    return nullptr;
}

State* GameOverState::HandleEvent(Event* event) {
    return nullptr;
}

void PauseState::Enter() {
    timer.Pause();
    Application::Get()->GetAudioManager().SetMusicVolume(50);
    Application::Get()->GetAudioManager().PlayMusicOverride("pause-screen",
                                                            true);
}

void PauseState::Exit() {}

void PauseState::Draw() {
    m_pause_screen.Draw();
}

State* PauseState::Update(float dt) {
    m_pause_screen.Update(*Application::Get()->GetPlayer());
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

void LevelUpState::Enter() {
    Renderer::GetInstance()->SetCameraTarget(GetGame()->GetPlayer());
    Application::Get()->GetAudioManager().MuteMusic();
    // save channel and  create callback. then halt sound on exit
    const int channel = Application::Get()->GetAudioManager().PlaySound(
        "thinking-music", 65, 10);
    if (channel != -1) {
        m_sounds_playing.push_back(channel);
    }
}

void LevelUpState::Exit() {
    Mix_ChannelFinished([](int /*channel*/) {
        Application::Get()->GetAudioManager().ToggleMusic();
        // Need to enable sound incase it was disabled by easter-egg
        Application::Get()->GetAudioManager().EnableSound();
        // remove callback
        Mix_ChannelFinished(nullptr);
    });

    for (const int channel : m_sounds_playing) {
        AudioManager::StopSound(channel);
    }
}

const std::unordered_map<std::string, Option> kLevelUpOptions = {
    {"damage",
     {"+10% Damage", "Increases damage by 10%",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetRangedDamage(stats.GetRangedDamage() +
                                stats.GetRangedDamage() * 0.1);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"health",
     {"+10% Health", "Increases health by 10%",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetMaxHealth(stats.GetMaxHealth() + stats.GetMaxHealth() * 0.1);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"speed",
     {"+10% Speed", "Increases speed by 10%",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetSpeed(stats.GetSpeed() + stats.GetSpeed() * 0.1);

          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"piercing",
     {"+1 Piercing", "Increases piercing by 1",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetPiercing(stats.GetPiercing() + 1);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"armor",
     {"+10% Armor", "Increases armor by 10%",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetArmorPercentage(stats.GetArmorPercentage() +
                                   stats.GetArmorPercentage() * 0.1);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"regen",
     {"+1 HP Regen", "Increases HP regen by 1",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetHpRegenRate(stats.GetHPRegenRate() + 1);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"lifesteal",
     {"+5% Lifesteal", "Increases lifesteal by 5%",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetLifeStealPercentage(stats.GetLifeStealPercentage() + 5);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"dodge",
     {"+1 Dodge", "Increases dodge by 1",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetDodgeCd(stats.GetDodgeCd() - 1);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"ranged",
     {"+5 Ranged Damage", "Increases ranged damage by 5",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetRangedDamage(stats.GetRangedDamage() + 5);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
    {"melee",
     {"+5 Melee Damage", "Increases melee damage by 5",
      []() {
          auto& stats = Application::Get()->GetPlayer()->GetMutableStats();

          stats.SetMeleeDamage(stats.GetMeleeDamage() + 5);
          PushNewEvent(EventType::LevelUpSelectedGameEvent);
      }}},
};

LevelUpState::LevelUpState(Game* game) : GameState(game) {
    {
        const int w = 200;
        const int h = 120;
        const int x = (Application::Get()->GetWindowWidth() - w * 2) / 3;
        const int y = (Application::Get()->GetWindowHeight() - h) / 2;

        for (auto const& [key, value] : kLevelUpOptions) {
            m_options.push_back(value);
        }

        // select 3 random options
        size_t opt_one_index = 0;
        size_t opt_two_index = 0;
        size_t opt_three_index = 0;
        while (opt_one_index == opt_two_index ||
               opt_one_index == opt_three_index ||
               opt_two_index == opt_three_index) {
            opt_one_index = rand() % m_options.size();
            opt_two_index = rand() % m_options.size();
            opt_three_index = rand() % m_options.size();
        }

        m_option_one_button = Button("buttons", SDL_Rect{x, y, w, h},
                                     m_options[opt_one_index].text,
                                     m_options[opt_one_index].side_effect);
        m_option_two_button = Button("buttons", SDL_Rect{x * 2, y, w, h},
                                     m_options[opt_two_index].text,
                                     m_options[opt_two_index].side_effect);
        m_option_three_button = Button("buttons", SDL_Rect{x * 3, y, w, h},
                                       m_options[opt_three_index].text,
                                       m_options[opt_three_index].side_effect);
    }
}

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

void ChestDropState::Enter() {
    timer.Pause();
    Application::Get()->GetAudioManager().SetMusicVolume(50);
    Application::Get()->GetAudioManager().PlayMusicOverride("pause-screen",
                                                            true);
}

void ChestDropState::Exit() {}

void ChestDropState::Draw() {
    GetGame()->DrawObjects();
    m_chest_drop_modal.Draw();
}

State* ChestDropState::Update(float dt) {
    m_chest_drop_modal.Update();
    GetGame()->UpdateObjects(dt);
    return nullptr;
}

State* ChestDropState::HandleEvent(Event* event) {
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

State* ShopState::HandleEvent(Event* /*event*/) {
    return nullptr;
}
