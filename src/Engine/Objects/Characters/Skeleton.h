#pragma once
#include "RangedEnemy.h"

class Skeleton : public RangedEnemy {
   public:
    explicit Skeleton(Properties& props, const RangedEnemyStats& stats);

    Skeleton(Collider* rhs, RangedEnemyStats stats);
    void Init();

    void Draw() override;
    void Update(float dt) override;

    GameObject* Copy() override;

    void Shoot() override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
};
