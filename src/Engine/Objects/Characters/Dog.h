#pragma once

#include "RangedEnemy.h"

class Dog : public RangedEnemy {
   public:
    explicit Dog(Properties& props, const RangedEnemyStats& stats);

    Dog(Collider* rhs, RangedEnemyStats stats);

    void Init();

    void Draw() override;
    void Update(float dt) override;

    GameObject* Copy() override;

    void Shoot() override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
};
