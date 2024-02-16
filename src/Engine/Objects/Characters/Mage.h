#pragma once

#include "RangedEnemy.h"

class Mage : public RangedEnemy {
   public:
    explicit Mage(Properties& props, const RangedEnemyStats& stats,
                  int bulletCount = 6);

    virtual void Draw() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_BulletCount;
};
