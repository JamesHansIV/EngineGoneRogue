#include "Engine/Objects/Slime.h"
#include "State.h"

class SlimeState : public State {
   public:
    explicit SlimeState(Slime* enemy) : m_Enemy(enemy) {}

    Slime* GetEnemy() { return m_Enemy; }

    void SetEnemy(Slime* enemy) { m_Enemy = enemy; }

   private:
    Slime* m_Enemy;
};

class SlimeIdle : public SlimeState {
   public:
    explicit SlimeIdle(Slime* enemy) : SlimeState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnTargetFoundEvent(TargetFoundEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Idle; }
};

class SlimeMoving : public SlimeState {
   public:
    explicit SlimeMoving(Slime* enemy) : SlimeState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Moving; }
};

class SlimeAttack : public SlimeState {
   public:
    explicit SlimeAttack(Slime* enemy) : SlimeState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Attack; }
};

class SlimeIsHit : public SlimeState {
   public:
    explicit SlimeIsHit(Slime* enemy, int damage)
        : SlimeState(enemy), m_Damage(damage) {}

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

class SlimeDead : public SlimeState {
   public:
    explicit SlimeDead(Slime* enemy) : SlimeState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Dead; }
};
