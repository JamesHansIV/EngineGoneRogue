#pragma once

#include "Engine/Objects/Enemy.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Objects/ProjectileManager.h"

class RangedEnemy : public Enemy {
   public:
    explicit RangedEnemy(Properties& props, int perceptionWidth,
                         int perceptionHeight, float range, int fireInterval)
        : Enemy(props, perceptionWidth, perceptionHeight, range),
          m_FireInterval(fireInterval) {
        m_Animation = new Animation();
        m_ProjectileManager = new ProjectileManager();
    }

    ~RangedEnemy();

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() = 0;

    int GetFireInterval() { return m_FireInterval; }

    ProjectileManager* GetProjectileManager() { return m_ProjectileManager; }

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    ProjectileManager* m_ProjectileManager;
    int m_FireInterval;
};
