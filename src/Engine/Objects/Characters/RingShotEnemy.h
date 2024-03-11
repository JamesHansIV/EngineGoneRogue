#pragma once

#include "RangedEnemy.h"

class RangedAttack;
const float kRingShotOuterRadius = 40.0F;
const float kRingShotInnerRadius = 20.0F;
const int kRingShotCount = 24;

class RingShotEnemy : public RangedEnemy {
   public:
    explicit RingShotEnemy(Properties& props, RangedEnemyStats stats,
                           float outerRadius = kRingShotOuterRadius,
                           float innerRadius = kRingShotInnerRadius,
                           int shotCount = kRingShotCount);

    RingShotEnemy(Collider* rhs, RangedEnemyStats stats,
                  float outerRadius = kRingShotOuterRadius,
                  float innerRadius = kRingShotInnerRadius,
                  int shotCount = kRingShotCount);

    void Init();

    virtual void Draw() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_shot_count;
    float m_outer_radius;
    float m_inner_radius;
};
