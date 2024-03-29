#pragma once

#include "Engine/Objects/Characters/Charger.h"
#include "Engine/State/EnemyState.h"
#include "Engine/Timer/Timer.h"
#include "State.h"

class ChargerAttack : public EnemyAttack {
   public:
    explicit ChargerAttack(Charger& enemy)
        : EnemyAttack(enemy), m_enemy(enemy) {}

    void Enter() override {

        if (timer.GetTicks() - m_enemy.GetLastAttack() <
            m_enemy.GetAttackInterval()) {
            SDL_Log("ChargerAttack::Enter() - Attack on cooldown");
            return;
        }
        m_enemy.SetLastAttack(timer.GetTicks());
        m_enemy.GetMutableEnemyStats().speed *= 2;
    }

    State* Update(float dt) override {
        if (timer.GetTicks() - m_enemy.GetLastAttack() >
            m_enemy.GetAttackDuration()) {
            SDL_Log("ChargerAttack::Enter() - Attack on cooldown");
            return m_enemy.GetIdleState();
        }

        m_enemy.MoveTowardsTarget(dt);

        return nullptr;
    }

    void Exit() override {
        m_enemy.GetMutableEnemyStats().speed = 70;
        SDL_Log("ChargerAttack::Exit()");
    }

    StateType GetType() override { return StateType::Attack; }

   protected:
    Charger& m_enemy;
};
