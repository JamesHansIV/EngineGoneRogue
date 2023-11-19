#pragma once

#include "Weapon.h"

class RangedWeapon : public Weapon {
    public:
        RangedWeapon(Properties& props, bool playerOwned);
        virtual void Draw() override;
        virtual void Clean() override;
        virtual void Update(float dt) override;

        void UpdateProjectiles(float dt);
        void UpdateColliders(const std::vector<Collider*>& colliders){m_Colliders = colliders;};

        virtual ObjectType GetObjectType() override { return ObjectType::kRangedWeapon; }
    private:
        std::vector<Projectile*> m_Projectiles;
        std::vector<Collider*> m_Colliders;
};