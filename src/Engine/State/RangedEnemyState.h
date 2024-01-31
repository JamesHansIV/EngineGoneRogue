
#include "Engine/Objects/Characters/RangedEnemy.h"
#include "State.h"

class RangedEnemyState : public State {
   public:
    explicit RangedEnemyState(RangedEnemy* enemy) : m_Enemy(enemy) {}

    RangedEnemy* GetEnemy() { return m_Enemy; }

    void SetEnemy(RangedEnemy* enemy) { m_Enemy = enemy; }

   private:
    RangedEnemy* m_Enemy;
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
        : RangedEnemyState(enemy), m_Direction(0) {}

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
    int m_Direction;
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
        : RangedEnemyState(enemy), m_Damage(damage) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::IsHit; }

    void SetDamage(int damage) { m_Damage = damage; }

    void ApplyDamage() { GetEnemy()->GetHealth()->SetDamage(m_Damage); }

   private:
    int m_Damage;
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
