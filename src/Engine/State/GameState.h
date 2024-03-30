#pragma once

#include <unordered_map>
#include <vector>
#include "Apps/Game.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/UI/Button.h"
#include "Engine/UI/ChestDropModal.h"
#include "Engine/UI/GameOverScreen.h"
#include "Engine/UI/PauseScreen.h"
#include "Engine/UI/StartScreen.h"
#include "State.h"

const int kDefaultTransitionTime = 500;

class Game;

class GameState : public State {
   public:
    explicit GameState(Game& game) : m_game(game) {}

    ~GameState() override = default;

   protected:
    Game& m_game;
};

class RunningState : public GameState {
   public:
    explicit RunningState(Game& game) : GameState(game) {}

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
    explicit StartState(Game& game) : GameState(game) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Start; }

   private:
    StartScreen m_start_screen;
};

class GameOverState : public GameState {
   public:
    explicit GameOverState(Game& game) : GameState(game) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::GameOver; }

   private:
    GameOverScreen m_game_over_screen;
};

class RoomTransitionState : public GameState {
   public:
    explicit RoomTransitionState(Game& game, std::string next_room_id)
        : GameState(game),
          m_next_room_id(next_room_id),
          m_enter_time(timer.GetTicks()),
          m_transition_time(kDefaultTransitionTime) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::RoomTransition; }

   private:
    std::string m_next_room_id;
    int m_enter_time;
    int m_transition_time;
};

struct Option {
    std::string text;
    std::string description;
    void (*side_effect)(Button&);
};

class LevelUpState : public GameState {
   public:
    explicit LevelUpState(Game& game);
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
    explicit PauseState(Game& game)
        : GameState(game), m_pause_screen(*game.GetPlayer()) {
        int const x = (Application::Get().GetWindowWidth() - 100) / 2;
        int const y = (Application::Get().GetWindowHeight() - 60) / 2;
        m_button = Button("buttons", SDL_Rect{x, y, 150, 80}, {"Continue"},
                          [](auto& button) {
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
    explicit ChestDropState(Game& game, std::vector<ItemType> items)
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
    explicit ShopState(Game& game) : GameState(game) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Shop; }

   private:
};
