#pragma once

#include "RangedEnemy.h"

class HelixEnemy : public RangedEnemy {
   public:
    explicit HelixEnemy(Properties& props, RangedEnemyStats const& stats);

    HelixEnemy(Collider* rhs, RangedEnemyStats stats);

    void Init();

    void Draw() override;
    void Update(float dt) override;

    GameObject* Copy() override;

    void Shoot() override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
};
