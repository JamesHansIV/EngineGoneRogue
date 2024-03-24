
#include "Engine/Objects/Characters/RangedEnemy.h"
#include "State.h"

class RangedEnemyState : public State {
   public:
    explicit RangedEnemyState(RangedEnemy* enemy) : m_enemy(enemy) {}

    RangedEnemy* GetEnemy() { return m_enemy; }

    void SetEnemy(RangedEnemy* enemy) { m_enemy = enemy; }

   private:
    RangedEnemy* m_enemy;
};

class RangedEnemyIdle : public RangedEnemyState {
   public:
    explicit RangedEnemyIdle(RangedEnemy* enemy) : RangedEnemyState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnTargetFoundEvent(TargetFoundEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Idle; }
};

class RangedEnemyMoving : public RangedEnemyState {
   public:
    explicit RangedEnemyMoving(RangedEnemy* enemy)
        : RangedEnemyState(enemy) {}

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

class RangedEnemyAttack : public RangedEnemyState {
   public:
    explicit RangedEnemyAttack(RangedEnemy* enemy) : RangedEnemyState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Attack; }
};

class RangedEnemyIsHit : public RangedEnemyState {
   public:
    explicit RangedEnemyIsHit(RangedEnemy* enemy, int damage)
        : RangedEnemyState(enemy), m_damage(damage) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::IsHit; }

    void SetDamage(int damage) { m_damage = damage; }

    void ApplyDamage() { GetEnemy()->GetHealth()->SetDamage(m_damage); }

   private:
    int m_damage;
};

class RangedEnemyDead : public RangedEnemyState {
   public:
    explicit RangedEnemyDead(RangedEnemy* enemy) : RangedEnemyState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Dead; }
};
