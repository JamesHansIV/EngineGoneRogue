#include "Engine/Objects/Characters/Slime.h"
#include "State.h"

class SlimeState : public State {
   public:
    explicit SlimeState(Slime* enemy) : m_enemy(enemy) {}

    Slime* GetEnemy() { return m_enemy; }

    void SetEnemy(Slime* enemy) { m_enemy = enemy; }

   private:
    Slime* m_enemy;
};

class SlimeIdle : public SlimeState {
   public:
    explicit SlimeIdle(Slime* enemy) : SlimeState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnTargetFoundEvent(TargetFoundEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Idle; }
};

class SlimeMoving : public SlimeState {
   public:
    explicit SlimeMoving(Slime* enemy) : SlimeState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Moving; }
};

class SlimeAttack : public SlimeState {
   public:
    explicit SlimeAttack(Slime* enemy) : SlimeState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Attack; }
};

class SlimeIsHit : public SlimeState {
   public:
    explicit SlimeIsHit(Slime* enemy, int damage)
        : SlimeState(enemy), m_damage(damage) {}

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

class SlimeDead : public SlimeState {
   public:
    explicit SlimeDead(Slime* enemy) : SlimeState(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Dead; }
};
