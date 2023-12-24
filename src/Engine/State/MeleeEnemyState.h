#include "Engine/Objects/MeleeEnemy.h"
#include "State.h"

class MeleeEnemyState : public State {
   public:
    explicit MeleeEnemyState(MeleeEnemy* enemy) : m_Enemy(enemy) {}

    MeleeEnemy* GetEnemy() { return m_Enemy; }

    void SetEnemy(MeleeEnemy* enemy) { m_Enemy = enemy; }

   private:
    MeleeEnemy* m_Enemy;
};

class MeleeEnemyIdle : public MeleeEnemyState {
   public:
    explicit MeleeEnemyIdle(MeleeEnemy* enemy) : MeleeEnemyState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnTargetFoundEvent(TargetFoundEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Idle; }
};

class MeleeEnemyMoving : public MeleeEnemyState {
   public:
    explicit MeleeEnemyMoving(MeleeEnemy* enemy) : MeleeEnemyState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Moving; }
};

class MeleeEnemyAttack : public MeleeEnemyState {
   public:
    explicit MeleeEnemyAttack(MeleeEnemy* enemy) : MeleeEnemyState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Attack; }
};

class MeleeEnemyIsHit : public MeleeEnemyState {
   public:
    explicit MeleeEnemyIsHit(MeleeEnemy* enemy, int damage)
        : MeleeEnemyState(enemy), m_Damage(damage) {}

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

class MeleeEnemyDead : public MeleeEnemyState {
   public:
    explicit MeleeEnemyDead(MeleeEnemy* enemy) : MeleeEnemyState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Dead; }
};
