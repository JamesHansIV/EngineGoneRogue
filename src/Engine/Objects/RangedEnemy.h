#pragma once

#include "Engine/Objects/Enemy.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Objects/ProjectileManager.h"

class RangedEnemy : public Enemy {
   public:
    explicit RangedEnemy(Properties& props, int perceptionWidth, int perceptionHeight, float range = 150.0f, int fireRate = 20);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    void SelectMoveAnimation();
    void HandleState(float dt);
    void Shoot();

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    ProjectileManager m_ProjectileManager;
    int m_FireRate;
};
