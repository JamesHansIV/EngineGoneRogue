#pragma once

#include "Enemy.h"

class MeleeEnemy : public Enemy {
   public:
    explicit MeleeEnemy(Properties& props, int perceptionWidth, int perceptionHeight, float range = 50.0f);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    void Attack();

    bool ManageState(float dt);

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
};
