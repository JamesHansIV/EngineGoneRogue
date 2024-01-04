#pragma once

#include "Engine/Objects/RangedEnemy.h"

class Mage : public RangedEnemy {
   public:
    explicit Mage(Properties& props, int perceptionWidth, int perceptionHeight,
                  float range = 150.0f, int fireInterval = 60,
                  int bulletCount = 6);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_BulletCount;
};
