#pragma once

#include "Apps/Game.h"
#include "Engine/Objects/Characters/Boss.h"
#include "Engine/Objects/Grenade.h"
#include "Engine/Objects/Projectiles/ProjectileManager.h"
#include "Engine/State/EnemyState.h"
#include "Engine/Timer/Timer.h"
#include "State.h"

const int kProjectileWidth = 32;
const int kProjectileHeight = 32;
const double kAttackDuration = 750;

class BossAttackUp : public EnemyAttack {
   public:
    explicit BossAttackUp(Boss& enemy) : EnemyAttack(enemy), m_enemy(enemy) {}

    void Enter() override {

        m_enemy.GetAnimation()->SelectAnimation("Attack-up");
    }

    State* Update(float dt) override {
        if (timer.GetTicks() - m_enemy.GetLastAttack() > kAttackDuration * 4) {
            m_enemy.SetAttacking(false);
            m_enemy.SetLastAttack(timer.GetTicks());
            return m_enemy.GetIdleState();
        }
        SDL_Log("Attack up");
        const AnimationInfo hit_animation_info = {
            "bullet-explosion", {0, 0, 16, 16}, 4, 150};

        auto* default_projectile_props =
            new Properties("weapons", {10, 5, 16, 16},
                           {m_enemy.GetMidPointX(), m_enemy.GetMidPointY(),
                            kProjectileWidth, kProjectileHeight},
                           m_enemy.GetRotation(), "bullet");

        auto* projectile =
            new Projectile(*default_projectile_props, 50, 3.1459 * 3 / 2,
                           hit_animation_info, false, 50, 0, nullptr);
        // Add player velocity to projectile. Have to multiply by .1 to make the
        // player velocity to be weaker than the projectile velocity, so that it does
        // not overpower the projectile velocity.
        projectile->SetVelocity(m_enemy.GetRigidBody()->Velocity() * .1 +
                                projectile->GetVelocity());
        ProjectileManager::GetInstance()->AddProjectile(projectile);
        ColliderHandler::GetInstance()->AddCollider(projectile);

        m_enemy.MoveTowardsTarget(dt);

        return nullptr;
    }

    void Exit() override {}

    StateType GetType() override { return StateType::Attack; }

   protected:
    Boss& m_enemy;
};

class BossAttackLeft : public EnemyAttack {
   public:
    explicit BossAttackLeft(Boss& enemy) : EnemyAttack(enemy), m_enemy(enemy) {}

    void Enter() override {
        m_enemy.GetAnimation()->SelectAnimation("Attack-left");
    }

    State* Update(float dt) override {
        if (timer.GetTicks() - m_enemy.GetLastAttack() > kAttackDuration * 3) {
            return new BossAttackUp(m_enemy);
        }
        const AnimationInfo hit_animation_info = {
            "bullet-explosion", {0, 0, 16, 16}, 4, 150};
        SDL_Log("Attack left");

        auto* default_projectile_props =
            new Properties("weapons", {10, 5, 16, 16},
                           {m_enemy.GetMidPointX(), m_enemy.GetMidPointY(),
                            kProjectileWidth, kProjectileHeight},
                           m_enemy.GetRotation(), "bullet");

        auto* projectile =
            new Projectile(*default_projectile_props, 50, 3.1459,
                           hit_animation_info, false, 50, 0, nullptr);
        // Add player velocity to projectile. Have to multiply by .1 to make the
        // player velocity to be weaker than the projectile velocity, so that it does
        // not overpower the projectile velocity.
        projectile->SetVelocity(m_enemy.GetRigidBody()->Velocity() * .1 +
                                projectile->GetVelocity());
        ProjectileManager::GetInstance()->AddProjectile(projectile);
        ColliderHandler::GetInstance()->AddCollider(projectile);

        m_enemy.MoveTowardsTarget(dt);

        return nullptr;
    }

    void Exit() override {}

    StateType GetType() override { return StateType::Attack; }

   protected:
    Boss& m_enemy;
};

class BossAttackDown : public EnemyAttack {
   public:
    explicit BossAttackDown(Boss& enemy) : EnemyAttack(enemy), m_enemy(enemy) {}

    void Enter() override {
        m_enemy.GetAnimation()->SelectAnimation("Attack-down");
    }

    State* Update(float dt) override {
        if (timer.GetTicks() - m_enemy.GetLastAttack() > kAttackDuration * 2) {
            return new BossAttackLeft(m_enemy);
        }
        const AnimationInfo hit_animation_info = {
            "bullet-explosion", {0, 0, 16, 16}, 4, 150};

        auto* default_projectile_props =
            new Properties("weapons", {10, 5, 16, 16},
                           {m_enemy.GetMidPointX(), m_enemy.GetMidPointY(),
                            kProjectileWidth, kProjectileHeight},
                           m_enemy.GetRotation(), "bullet");

        auto* projectile =
            new Projectile(*default_projectile_props, 50, 3.1459 / 2,
                           hit_animation_info, false, 50, 0, nullptr);
        // Add player velocity to projectile. Have to multiply by .1 to make the
        // player velocity to be weaker than the projectile velocity, so that it does
        // not overpower the projectile velocity.
        projectile->SetVelocity(m_enemy.GetRigidBody()->Velocity() * .1 +
                                projectile->GetVelocity());
        ProjectileManager::GetInstance()->AddProjectile(projectile);
        ColliderHandler::GetInstance()->AddCollider(projectile);

        m_enemy.MoveTowardsTarget(dt);

        return nullptr;
    }

    void Exit() override {}

    StateType GetType() override { return StateType::Attack; }

   protected:
    Boss& m_enemy;
};

class BossAttack : public EnemyAttack {
   public:
    explicit BossAttack(Boss& enemy) : EnemyAttack(enemy), m_enemy(enemy) {}

    void Enter() override {
        if (!m_enemy.IsAttacking() &&
            timer.GetTicks() - m_enemy.GetLastAttack() <
                m_enemy.GetAttackInterval()) {
            m_should_skip = true;
            return;
        }

        if (!m_enemy.IsAttacking()) {
            m_enemy.SetLastAttack(timer.GetTicks());
        }

        m_enemy.SetAttacking(true);
        m_enemy.GetAnimation()->SelectAnimation("Attack-right");
    }

    State* Update(float dt) override {
        if (m_should_skip) {
            return new EnemyIdle(m_enemy);
        }

        if (timer.GetTicks() - m_enemy.GetLastAttack() > kAttackDuration) {
            return new BossAttackDown(m_enemy);
        }

        const AnimationInfo hit_animation_info = {
            "bullet-explosion", {0, 0, 16, 16}, 4, 150};

        auto* default_projectile_props =
            new Properties("weapons", {10, 5, 16, 16},
                           {m_enemy.GetMidPointX(), m_enemy.GetMidPointY(),
                            kProjectileWidth, kProjectileHeight},
                           m_enemy.GetRotation(), "bullet");

        auto* projectile =
            new Projectile(*default_projectile_props, 50, m_enemy.GetRadians(),
                           hit_animation_info, false, 50, 0, nullptr);
        // Add player velocity to projectile. Have to multiply by .1 to make the
        // player velocity to be weaker than the projectile velocity, so that it does
        // not overpower the projectile velocity.
        projectile->SetVelocity(m_enemy.GetRigidBody()->Velocity() * .1 +
                                projectile->GetVelocity());
        ProjectileManager::GetInstance()->AddProjectile(projectile);
        ColliderHandler::GetInstance()->AddCollider(projectile);

        m_enemy.MoveTowardsTarget(dt);

        return nullptr;
    }

    void Exit() override {}

    StateType GetType() override { return StateType::Attack; }

   protected:
    Boss& m_enemy;

   private:
    bool m_should_skip = false;
};
