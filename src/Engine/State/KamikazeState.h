#pragma once

#include "Engine/Objects/Characters/Kamikaze.h"
#include "Engine/State/EnemyState.h"
#include "Engine/Timer/Timer.h"
#include "State.h"

class KamikazeIdle : public EnemyIdle {
   public:
    explicit KamikazeIdle(Enemy& enemy) : EnemyIdle(enemy) {}
};

class KamikazeMoving : public EnemyMoving {
   public:
    explicit KamikazeMoving(Enemy& enemy) : EnemyMoving(enemy) {}

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

class KamikazeAttack : public EnemyAttack {
   public:
    explicit KamikazeAttack(Kamikaze& enemy)
        : EnemyAttack(enemy), m_enemy(enemy) {}

    void Enter() override {

        if (timer.GetTicks() - m_enemy.GetLastAttack() <
            m_enemy.GetAttackInterval()) {
            SDL_Log("KamikazeAttack::Enter() - Attack on cooldown");
            return;
        }
        m_enemy.SetLastAttack(timer.GetTicks());
        m_enemy.GetMutableEnemyStats().speed *= 2;
    }

    State* Update(float dt) override {
        if (timer.GetTicks() - m_enemy.GetLastAttack() >
            m_enemy.GetAttackDuration()) {
            SDL_Log("KamikazeAttack::Enter() - Attack on cooldown");
            return m_enemy.GetIdleState();
        }

        m_enemy.MoveTowardsTarget(dt);

        return nullptr;
    }

    void Exit() override {
        m_enemy.GetMutableEnemyStats().speed = 70;
        SDL_Log("KamikazeAttack::Exit()");
    }

    StateType GetType() override { return StateType::Attack; }

   protected:
    Kamikaze& m_enemy;
};

class KamikazeDead : public EnemyDead {
   public:
    explicit KamikazeDead(Enemy& enemy) : EnemyDead(enemy) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;
    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Dead; }
};
