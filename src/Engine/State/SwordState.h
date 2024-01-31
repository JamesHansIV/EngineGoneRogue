#include "Engine/Objects/Weapons/Sword.h"
#include "State.h"

class SwordState : public State {
   public:
    explicit SwordState(Sword* sword) : m_Sword(sword) {}

    Sword* GetSword() { return m_Sword; }

   private:
    Sword* m_Sword;
};

class SwordIdle : public SwordState {
   public:
    explicit SwordIdle(Sword* sword) : SwordState(sword) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Idle; }
};

class SwordAttack : public SwordState {
   public:
    explicit SwordAttack(Sword* sword) : SwordState(sword) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual State* Update(float dt) override;
    virtual void Draw() override;
    virtual State* HandleEvent(Event* event) override;

    virtual StateType GetType() override { return StateType::Attack; }
};
