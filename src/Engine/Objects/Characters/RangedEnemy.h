#pragma once

#include "Enemy.h"
#include "Engine/Objects/Attacks/AttackFunctions.h"

class RangedEnemy : public Enemy {
   public:
    explicit RangedEnemy(Properties& props, const RangedEnemyStats& enemyStats)
        : Enemy(props, enemyStats), m_stats(enemyStats) {}

    RangedEnemy(Collider* rhs, RangedEnemyStats stats);

    ~RangedEnemy() override;

    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    virtual void Shoot() = 0;

    [[nodiscard]] int GetFireInterval() const { return m_stats.fireInterval; }

    RangedAttack* GetAttack() { return m_attack; }

    void SetAttack(RangedAttack* attack) { m_attack = attack; }

    RangedEnemyStats GetRangedEnemyStats() { return m_stats; }

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   protected:
    RangedAttack* m_attack;
    RangedEnemyStats m_stats;
};
