#pragma once

#include "Engine/Objects/Characters/Enemy.h"
#include "State.h"

class EnemyState : public State {
   public:
    explicit EnemyState(Enemy& enemy) : m_enemy(enemy) {}

    void SetEnemy(Enemy& enemy) { m_enemy = enemy; }

   protected:
    Enemy& m_enemy;
};

class EnemyIdle : public EnemyState {
   public:
    explicit EnemyIdle(Enemy& enemy) : EnemyState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnTargetFoundEvent(TargetFoundEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Idle; }
};

class EnemyMoving : public EnemyState {
   public:
    explicit EnemyMoving(Enemy& enemy) : EnemyState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    void SelectDirection();

    StateType GetType() override { return StateType::Moving; }

   private:
    //0 is up and 1 is down
    int m_direction{0};
};

class EnemyAttack : public EnemyState {
   public:
    explicit EnemyAttack(Enemy& enemy) : EnemyState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Attack; }
};

class EnemyIsHit : public EnemyState {
   public:
    explicit EnemyIsHit(Enemy& enemy, int damage)
        : EnemyState(enemy), m_damage(damage) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;
    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::IsHit; }

    void SetDamage(int damage) { m_damage = damage; }

    void ApplyDamage() { m_enemy.GetHealth()->SetDamage(m_damage); }

   private:
    int m_damage;
};

class EnemyDead : public EnemyState {
   public:
    explicit EnemyDead(Enemy& enemy) : EnemyState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;
    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Dead; }
};
