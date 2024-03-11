
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

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnTargetFoundEvent(TargetFoundEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Idle; }
};

class RangedEnemyMoving : public RangedEnemyState {
   public:
    explicit RangedEnemyMoving(RangedEnemy* enemy)
        : RangedEnemyState(enemy), m_direction(0) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    void SelectDirection();

    virtual StateType GetType() override { return StateType::Moving; }

   private:
    //0 is up and 1 is down
    int m_direction;
};

class RangedEnemyAttack : public RangedEnemyState {
   public:
    explicit RangedEnemyAttack(RangedEnemy* enemy) : RangedEnemyState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Attack; }
};

class RangedEnemyIsHit : public RangedEnemyState {
   public:
    explicit RangedEnemyIsHit(RangedEnemy* enemy, int damage)
        : RangedEnemyState(enemy), m_damage(damage) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::IsHit; }

    void SetDamage(int damage) { m_damage = damage; }

    void ApplyDamage() { GetEnemy()->GetHealth()->SetDamage(m_damage); }

   private:
    int m_damage;
};

class RangedEnemyDead : public RangedEnemyState {
   public:
    explicit RangedEnemyDead(RangedEnemy* enemy) : RangedEnemyState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Dead; }
};
