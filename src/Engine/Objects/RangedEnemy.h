#pragma once

#include "Engine/Objects/Enemy.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Objects/ProjectileManager.h"

class RangedEnemy : public Enemy {
   public:
    explicit RangedEnemy(Properties& props, int perceptionWidth, int perceptionHeight);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    void Shoot();

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    Animation* m_Animation;
    ProjectileManager m_ProjectileManager;
    std::vector<Projectile*> m_Projectiles;
};
