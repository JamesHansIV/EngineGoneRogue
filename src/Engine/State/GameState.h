#pragma once

#include "Engine/UI/Button.h"
#include "State.h"

class Game;

class GameState : public State {
   public:
    GameState(Game* game) : m_Game(game) {}

    virtual ~GameState() {}

    Game* GetGame() { return m_Game; }

   private:
    Game* m_Game;
};

class RunningState : public GameState {
   public:
    RunningState(Game* game) : GameState(game) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Running; }

   private:
};

class StartState : public GameState {
   public:
    StartState(Game* game) : GameState(game) {
        int x = (Application::Get()->GetWindowWidth() - 100) / 2;
        int y = (Application::Get()->GetWindowHeight() - 60) / 2;
        m_button = Button(SDL_Rect{x, y, 100, 60}, "Start", []() {
            SDL_Log("Start button clicked");
            timer.Unpause();
            PushNewEvent(EventType::StartGameEvent);
        });
    }

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Start; }

   private:
    Button m_button;
};

class GameOverState : public GameState {
   public:
    GameOverState(Game* game) : GameState(game) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::GameOver; }

   private:
};

class PauseState : public GameState {
   public:
    PauseState(Game* game) : GameState(game) {
        int x = (Application::Get()->GetWindowWidth() - 100) / 2;
        int y = (Application::Get()->GetWindowHeight() - 60) / 2;
        m_button = Button(SDL_Rect{x, y, 100, 60}, "Continue", []() {
            SDL_Log("Continue button clicked");
            timer.Unpause();
            PushNewEvent(EventType::ContinueGameEvent);
        });
    }

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Pause; }

   private:
    Button m_button;
};

class ShopState : public GameState {
   public:
    ShopState(Game* game) : GameState(game) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Shop; }

   private:
};
