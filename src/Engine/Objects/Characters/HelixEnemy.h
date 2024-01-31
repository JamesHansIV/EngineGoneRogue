#pragma once

#include "RangedEnemy.h"

class HelixEnemy : public RangedEnemy {
   public:
    explicit HelixEnemy(Properties& props, int perceptionWidth,
                        int perceptionHeight, float range = 150.0f,
                        int fireInterval = 20);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
};
