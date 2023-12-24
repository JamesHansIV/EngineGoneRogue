#pragma once

#include <SDL2/SDL.h>
#include "Engine/Events/Event.h"
#include "Engine/State/State.h"

class Player;

class PlayerState : public State {
   public:
    explicit PlayerState(Player* player) : m_Player(player) {}

    virtual ~PlayerState() {}

    Player* GetPlayer() { return m_Player; }

    void SetPlayer(Player* p) { m_Player = p; }

   private:
    Player* m_Player;
};

class PlayerIdle : public PlayerState {
   public:
    explicit PlayerIdle(Player* player) : PlayerState(player) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Idle; }
};

class PlayerMoving : public PlayerState {
   public:
    explicit PlayerMoving(Player* player) : PlayerState(player) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Moving; }
};

class PlayerIsHit : public PlayerState {
   public:
    explicit PlayerIsHit(Player* player, int damage)
        : PlayerState(player), m_Damage(damage) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::IsHit; }

    void SetDamage(int damage) { m_Damage = damage; }

    void ApplyDamage();

   private:
    int m_Damage;
};

class PlayerDead : public PlayerState {
   public:
    explicit PlayerDead(Player* player) : PlayerState(player) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Dead; }
};
