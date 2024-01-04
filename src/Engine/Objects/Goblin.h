#pragma once

#include "Engine/Objects/RangedEnemy.h"

class Goblin : public RangedEnemy {
   public:
    explicit Goblin(Properties& props, int perceptionWidth,
                    int perceptionHeight, float range = 150.0f,
                    int fireInterval = 7, float spread = M_PI / 3);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    float GetSpread() const { return m_Spread; }

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    float m_Spread;
};
