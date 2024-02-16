#pragma once

#include "RangedEnemy.h"

class RangedAttack;

class CircleShotEnemy : public RangedEnemy {
   public:
    explicit CircleShotEnemy(Properties& props, RangedEnemyStats stats,
                             float outerRadius = 40.0f,
                             float innerRadius = 20.0f, int shotCount = 24);

    void Draw() override;
    void Update(float dt) override;

    void Shoot() override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_ShotCount;
    float m_OuterRadius;
    float m_InnerRadius;
};
