#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "Engine/Events/Event.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Physics/Vector2D.h"
#include "Engine/State/State.h"

class Player;

struct AnimationIDs {
    std::string Up;
    std::string LeftUp;
    std::string Left;
    std::string RightUp;
    std::string Right;
    std::string Down;
};

class PlayerState : public State {
   public:
    explicit PlayerState(Player* player) : m_Player(player) {
        m_Move.Up = "move-up";
        m_Move.LeftUp = "move-left-up";
        m_Move.Left = "move-left";
        m_Move.RightUp = "move-right-up";
        m_Move.Right = "move-right";
        m_Move.Down = "move-down";

        m_Dodge.Up = "dodge-up";
        m_Dodge.LeftUp = "dodge-left-up";
        m_Dodge.Left = "dodge-left";
        m_Dodge.RightUp = "dodge-right-up";
        m_Dodge.Right = "dodge-right";
        m_Dodge.Down = "dodge-down";
    }

    virtual ~PlayerState() {}

    Player* GetPlayer() { return m_Player; }

    void SetPlayer(Player* p) { m_Player = p; }

    AnimationIDs& GetMoveAnimationIDs() { return m_Move; }

    AnimationIDs& GetDodgeAnimationIDs() { return m_Dodge; }

   private:
    Player* m_Player;
    AnimationIDs m_Move;
    AnimationIDs m_Dodge;
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

    State* PollInput(float dt);

    virtual StateType GetType() override { return StateType::Idle; }
};

//TODO: player move up and down animations skip some frames...
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

    void PollInput(float dt);

    virtual StateType GetType() override { return StateType::Moving; }
};

class PlayerDodge : public PlayerState {
   public:
    explicit PlayerDodge(Player* player)
        : PlayerState(player), m_DodgeCD(60), m_Velocity(0, 0) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Dodge; }

   private:
    int m_DodgeCD;
    float m_Distance;
    Vector2D m_Velocity;
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

    static State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    void PollInput(float dt);

    void SetDamage(int damage) { m_Damage = damage; }

    void ApplyDamage();

    virtual StateType GetType() override { return StateType::IsHit; }

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
