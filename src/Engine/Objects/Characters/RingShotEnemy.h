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

    void Draw() override;
    void Update(float dt) override;

    GameObject* Copy() override;

    void Shoot() override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_shot_count;
    float m_outer_radius;
    float m_inner_radius;
};
