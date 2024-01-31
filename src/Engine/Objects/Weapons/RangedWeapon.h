#pragma once

#include "Engine/Objects/Projectiles/ProjectileManager.h"
#include "Weapon.h"

class RangedWeapon : public Weapon {
   public:
    RangedWeapon(Properties& props, bool playerOwned);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void UpdateProjectiles(float dt);

    void SetFireRate(Uint32 fire_rate) { m_fire_rate = fire_rate; }

    virtual ObjectType GetObjectType() override {
        return ObjectType::RangedWeapon;
    }

   private:
    ProjectileManager m_ProjectileManager;
    bool m_auto_fire_enabled = false;
    Uint32 m_fire_rate = 200;
    Uint32 m_last_fired = 0;
};
