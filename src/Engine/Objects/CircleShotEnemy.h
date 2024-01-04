#pragma once

#include "Engine/Objects/RangedEnemy.h"

class CircleShotEnemy : public RangedEnemy {
   public:
    explicit CircleShotEnemy(Properties& props, int perceptionWidth,
                             int perceptionHeight, float range = 150.0f,
                             int fireInterval = 80, float outerRadius = 40.0f,
                             float innerRadius = 20.0f, int shotCount = 24);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_ShotCount;
    float m_OuterRadius;
    float m_InnerRadius;
};
