#pragma once

#include <vector>
#include "Engine/Events/Event.h"
#include "Engine/Objects/ChestDropModal.h"
#include "Engine/UI/Button.h"
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
    explicit StartState(Game* game) : GameState(game) {
        int const x = (Application::Get()->GetWindowWidth() - 100) / 2;
        int const y = (Application::Get()->GetWindowHeight() - 60) / 2;
        m_button = Button(SDL_Rect{x, y, 100, 60}, "Start", []() {
            SDL_Log("Start button clicked");
            timer.Unpause();
            PushNewEvent(EventType::StartGameEvent);
        });
    }

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Start; }

   private:
    Button m_button;
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
};

class LevelUpState : public GameState {
   public:
    explicit LevelUpState(Game* game) : GameState(game) {
        const int w = 200;
        const int h = 120;
        const int x = (Application::Get()->GetWindowWidth() - w * 2) / 3;
        const int y = (Application::Get()->GetWindowHeight() - h) / 2;
        // Todo: Have LevelUpSelected have functionality to add the side effects
        // of the level up option selected
        m_option_one_button = Button(SDL_Rect{x, y, w, h}, "OptionOne", []() {
            SDL_Log("OptionOne button clicked");
            timer.Unpause();
            PushNewEvent(EventType::LevelUpSelectedGameEvent);
        });
        m_option_two_button =
            Button(SDL_Rect{x * 2, y, w, h}, "OptionTwo", []() {
                SDL_Log("OptionTwo button clicked");
                timer.Unpause();
                PushNewEvent(EventType::LevelUpSelectedGameEvent);
            });
        m_option_three_button =
            Button(SDL_Rect{x * 3, y, w, h}, "OptionThree", []() {
                SDL_Log("OptionThree button clicked");
                timer.Unpause();
                PushNewEvent(EventType::LevelUpSelectedGameEvent);
            });
    }

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
};

class PauseState : public GameState {
   public:
    explicit PauseState(Game* game) : GameState(game) {
        int const x = (Application::Get()->GetWindowWidth() - 100) / 2;
        int const y = (Application::Get()->GetWindowHeight() - 60) / 2;
        m_button = Button(SDL_Rect{x, y, 100, 60}, "Continue", []() {
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
    Button m_button;
};

class ChestDropState : public GameState {
   public:
    explicit ChestDropState(Game* game, std::vector<ItemType> items) : GameState(game) {
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
