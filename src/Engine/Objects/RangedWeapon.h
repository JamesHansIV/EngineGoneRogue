#pragma once

#include "Weapon.h"
#include "ProjectileManager.h"

class RangedWeapon : public Weapon {
   public:
    RangedWeapon(Properties& props, bool playerOwned);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    void UpdateProjectiles(float dt);

    virtual ObjectType GetObjectType() override {
        return ObjectType::RangedWeapon;
    }

   private:
    ProjectileManager m_ProjectileManager;
    bool m_auto_fire_enabled = false;
};