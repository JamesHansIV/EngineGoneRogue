#pragma once

#include "Apps/Game.h"
#include "Engine/Objects/Characters/Kamikaze.h"
#include "Engine/Objects/Grenade.h"
#include "Engine/State/EnemyState.h"
#include "Engine/Timer/Timer.h"
#include "State.h"

class KamikazeAttack : public EnemyAttack {
   public:
    explicit KamikazeAttack(Kamikaze& enemy)
        : EnemyAttack(enemy), m_enemy(enemy) {}

    void Enter() override {

        m_enemy.SetLastAttack(timer.GetTicks());
        m_enemy.GetMutableEnemyStats().speed *= 2;
        m_enemy.GetAnimation()->SelectAnimation("Attack-down");
        SDL_Log("KamikazeAttack::Enter()");
    }

    State* Update(float dt) override {
        if (timer.GetTicks() - m_enemy.GetLastAttack() >
            m_enemy.GetAttackDuration()) {
            Properties grenade_props("weapons", {10, 3, 16, 16},
                                     {m_enemy.GetX(), m_enemy.GetY(), 36, 36},
                                     0.0);

            auto* grenade =
                new Grenade(grenade_props,
                            {80 + m_enemy.GetEnemyStats().damage, 0.1, 50, 50});
            static_cast<Game&>(Application::Get()).AddObject(grenade);
            ColliderHandler::GetInstance()->AddCollider(grenade);

            return m_enemy.GetDeadState();
        }

        SDL_Log("KamikazeAttack::Update()");

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
