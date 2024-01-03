#include "Engine/Objects/Dog.h"
#include "State.h"

class DogState : public State {
   public:
    explicit DogState(Dog* enemy) : m_Enemy(enemy) {}

    Dog* GetEnemy() { return m_Enemy; }

    void SetEnemy(Dog* enemy) { m_Enemy = enemy; }

   private:
    Dog* m_Enemy;
};

class DogIdle : public DogState {
   public:
    explicit DogIdle(Dog* enemy) : DogState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnTargetFoundEvent(TargetFoundEvent* event);
    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Idle; }
};

class DogMoving : public DogState {
   public:
    explicit DogMoving(Dog* enemy) : DogState(enemy), m_Direction(0) {}

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

class DogAttack : public DogState {
   public:
    explicit DogAttack(Dog* enemy) : DogState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    virtual StateType GetType() override { return StateType::Attack; }
};

class DogIsHit : public DogState {
   public:
    explicit DogIsHit(Dog* enemy, int damage)
        : DogState(enemy), m_Damage(damage) {}

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

class DogDead : public DogState {
   public:
    explicit DogDead(Dog* enemy) : DogState(enemy) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Dead; }
};
