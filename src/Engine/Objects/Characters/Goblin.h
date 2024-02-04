#pragma once

#include "RangedEnemy.h"

class Goblin : public RangedEnemy {
   public:
    explicit Goblin(Properties& props, const RangedEnemyStats& enemyStats);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    float GetSpread() const { return m_stats.spread; }

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }
};
