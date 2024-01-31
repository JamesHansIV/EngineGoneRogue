#pragma once

#include "Enemy.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Objects/Projectiles/ProjectileManager.h"

struct Burst {
    int _Count;
    float _CD;
    int Count;
    float CD;

    void ResetCount() { Count = _Count; }

    void ResetCD() { CD = _CD; }

    void Reset() {
        ResetCount();
        ResetCD();
    }

    void Update(float dt) {
        if (Count <= 0) {
            if (CD > 0) {
                CD--;
            } else {
                ResetCount();
                ResetCD();
            }
        }
    }

    bool Fire() {
        if (Count > 0) {
            Count--;
            return true;
        }
        return false;
    }

    Burst(int count, float cd) : _Count(count), Count(count), _CD(cd), CD(cd) {}
};

class RangedEnemy : public Enemy {
   public:
    explicit RangedEnemy(Properties& props, int perceptionWidth,
                         int perceptionHeight, float range, int fireInterval,
                         Burst* burst)
        : Enemy(props, perceptionWidth, perceptionHeight, range),
          m_FireInterval(fireInterval),
          m_Burst(burst) {
        m_Animation = new Animation();
        m_ProjectileManager = new ProjectileManager();
    }

    ~RangedEnemy();

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() = 0;

    int GetFireInterval() { return m_FireInterval; }

    Burst* GetBurst() { return m_Burst; }

    ProjectileManager* GetProjectileManager() { return m_ProjectileManager; }

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    ProjectileManager* m_ProjectileManager;
    int m_FireInterval;
    Burst* m_Burst;
};
