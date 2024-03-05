#pragma once

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
    StartState(Game* game) : GameState(game) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Start; }

   private:
    int m_ButtonX;
    int m_ButtonY;
    int m_ButtonW;
    int m_ButtonH;
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
    PauseState(Game* game) : GameState(game) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Pause; }

   private:
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
