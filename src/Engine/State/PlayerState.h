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
    explicit PlayerState(Player* player) : m_player(player) {
        m_move.Up = "move-up";
        m_move.LeftUp = "move-left-up";
        m_move.Left = "move-left";
        m_move.RightUp = "move-right-up";
        m_move.Right = "move-right";
        m_move.Down = "move-down";

        m_dodge.Up = "dodge-up";
        m_dodge.LeftUp = "dodge-left-up";
        m_dodge.Left = "dodge-left";
        m_dodge.RightUp = "dodge-right-up";
        m_dodge.Right = "dodge-right";
        m_dodge.Down = "dodge-down";
    }

    ~PlayerState() override = default;

    Player* GetPlayer() { return m_player; }

    void SetPlayer(Player* p) { m_player = p; }

    AnimationIDs& GetMoveAnimationIDs() { return m_move; }

    AnimationIDs& GetDodgeAnimationIDs() { return m_dodge; }

   private:
    Player* m_player;
    AnimationIDs m_move;
    AnimationIDs m_dodge;
};

class PlayerIdle : public PlayerState {
   public:
    explicit PlayerIdle(Player* player) : PlayerState(player) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    State* PollInput(float dt);

    StateType GetType() override { return StateType::Idle; }
};

//TODO: player move up and down animations skip some frames...
class PlayerMoving : public PlayerState {
   public:
    explicit PlayerMoving(Player* player) : PlayerState(player) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    void PollInput(float dt);

    StateType GetType() override { return StateType::Moving; }
};

class PlayerDodge : public PlayerState {
   public:
    explicit PlayerDodge(Player* player)
        : PlayerState(player),  m_velocity(0, 0) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Dodge; }

   private:
    int m_dodge_cd{60};
    float m_distance;
    Vector2D m_velocity;
};

class PlayerIsHit : public PlayerState {
   public:
    explicit PlayerIsHit(Player* player, int damage)
        : PlayerState(player), m_damage(damage) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    static State* OnUserEvent(UserEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    void PollInput(float dt);

    void SetDamage(int damage) { m_damage = damage; }

    void ApplyDamage();

    StateType GetType() override { return StateType::IsHit; }

   private:
    int m_damage;
};

class PlayerDead : public PlayerState {
   public:
    explicit PlayerDead(Player* player) : PlayerState(player) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Dead; }
};
