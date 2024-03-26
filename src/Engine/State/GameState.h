#pragma once

#include <unordered_map>
#include <vector>
#include "Engine/Events/Event.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/UI/ChestDropModal.h"
#include "Engine/UI/Button.h"
#include "Engine/UI/GameOverScreen.h"
#include "Engine/UI/PauseScreen.h"
#include "Engine/UI/StartScreen.h"
#include "State.h"

class Game;

class GameState : public State {
   public:
    explicit GameState(Game* game) : m_game(game) {}

    ~GameState() override = default;

    Game* GetGame() { return m_game; }

   private:
    Game* m_game;
};

class RunningState : public GameState {
   public:
    explicit RunningState(Game* game) : GameState(game) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Running; }

   private:
};

class StartState : public GameState {
   public:
    explicit StartState(Game* game) : GameState(game) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Start; }

   private:
    StartScreen m_startScreen;
};

class GameOverState : public GameState {
   public:
    explicit GameOverState(Game* game) : GameState(game) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::GameOver; }

   private:
    GameOverScreen m_game_over_screen;
};

struct Option {
    std::string text;
    std::string description;
    void (*side_effect)();
};

class LevelUpState : public GameState {
   public:
    explicit LevelUpState(Game* game);
    void Enter() override;

    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::LevelUp; }

   private:
    Button m_option_one_button;
    Button m_option_two_button;
    Button m_option_three_button;
    std::vector<int> m_sounds_playing;
    std::vector<Option> m_options;
};

class PauseState : public GameState {
   public:
    explicit PauseState(Game* game)
        : GameState(game), m_pause_screen(*Application::Get()->GetPlayer()) {
        int const x = (Application::Get()->GetWindowWidth() - 100) / 2;
        int const y = (Application::Get()->GetWindowHeight() - 60) / 2;
        m_button = Button("buttons", SDL_Rect{x, y, 150, 80}, "Continue", []() {
            SDL_Log("Continue button clicked");
            timer.Unpause();
            PushNewEvent(EventType::ContinueGameEvent);
        });
    }

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Pause; }

   private:
    PauseScreen m_pause_screen;
    Button m_button;
};

class ChestDropState : public GameState {
   public:
    explicit ChestDropState(Game* game, std::vector<ItemType> items)
        : GameState(game) {
        m_chest_drop_modal = ChestDropModal(items);
    }

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::ChestDrop; }

   private:
    ChestDropModal m_chest_drop_modal;
};

class ShopState : public GameState {
   public:
    explicit ShopState(Game* game) : GameState(game) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Shop; }

   private:
};
