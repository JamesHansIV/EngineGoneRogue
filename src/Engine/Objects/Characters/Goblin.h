#pragma once

#include "RangedEnemy.h"

class Goblin : public RangedEnemy {
   public:
    explicit Goblin(Properties& props, const RangedEnemyStats& enemyStats);

    Goblin(Collider* rhs, RangedEnemyStats stats);

    void Init();

    void Draw() override;
    void Update(float dt) override;

    void Shoot() override;

    void OnCollide(Collider* collidee) override;

    [[nodiscard]] float GetSpread() const { return m_stats.spread; }

    ObjectType GetObjectType() override { return ObjectType::Enemy; }
};
