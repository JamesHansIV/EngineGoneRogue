#pragma once

#include "RangedEnemy.h"

class Mage : public RangedEnemy {
   public:
    explicit Mage(Properties& props, const RangedEnemyStats& stats,
                  int bulletCount = 6);

    Mage(Collider* rhs, RangedEnemyStats stats, int bulletCount = 6);

    void Init();

    void Draw() override;
    void Update(float dt) override;

    GameObject* Copy() override;

    void Shoot() override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_bullet_count;
};
