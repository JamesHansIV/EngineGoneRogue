#pragma once

#include "Enemy.h"
#include "Engine/Objects/Attacks/AttackFunctions.h"

class RangedEnemy : public Enemy {
   public:
    explicit RangedEnemy(Properties& props, const RangedEnemyStats& enemyStats)
        : Enemy(props, enemyStats), m_stats(enemyStats) {}

    RangedEnemy(Collider* rhs, RangedEnemyStats stats);

    ~RangedEnemy();

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() = 0;

    int GetFireInterval() const { return m_stats.fireInterval; }

    RangedAttack* GetAttack() { return m_Attack; }

    void SetAttack(RangedAttack* attack) { m_Attack = attack; }

    RangedEnemyStats GetRangedEnemyStats() { return m_stats; }

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   protected:
    RangedAttack* m_Attack;
    RangedEnemyStats m_stats;
};
