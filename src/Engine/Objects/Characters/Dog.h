#pragma once

#include "RangedEnemy.h"

class Dog : public RangedEnemy {
   public:
    explicit Dog(Properties& props, const RangedEnemyStats& stats);

    virtual void Draw() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
};
