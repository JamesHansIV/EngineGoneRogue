#pragma once

#include "RangedEnemy.h"

class RangedAttack;

class RingShotEnemy : public RangedEnemy {
   public:
    explicit RingShotEnemy(Properties& props, RangedEnemyStats stats,
                           float outerRadius = 40.0f, float innerRadius = 20.0f,
                           int shotCount = 24);

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
