#pragma once

#include "RangedEnemy.h"

class RangedAttack;
const float RING_SHOT_OUTER_RADIUS = 40.0f;
const float RING_SHOT_INNER_RADIUS = 20.0f;
const int RING_SHOT_COUNT = 24;

class RingShotEnemy : public RangedEnemy {
   public:
    explicit RingShotEnemy(Properties& props, RangedEnemyStats stats,
                           float outerRadius = RING_SHOT_OUTER_RADIUS,
                           float innerRadius = RING_SHOT_INNER_RADIUS,
                           int shotCount = RING_SHOT_COUNT);

    RingShotEnemy(Collider& rhs, RangedEnemyStats stats,
                  float outerRadius = RING_SHOT_OUTER_RADIUS,
                  float innerRadius = RING_SHOT_INNER_RADIUS,
                  int shotCount = RING_SHOT_COUNT);

    void Init();

    virtual void Draw() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_ShotCount;
    float m_OuterRadius;
    float m_InnerRadius;
};
