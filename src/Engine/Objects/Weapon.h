#pragma once
#include "GameObject.h"
#include "Engine/Input/InputChecker.h"
#include "Projectile.h"
#include "Engine/Objects/Collider.h"

/*
    notes:
    weapon type - enum?
    texture
*/

enum WeaponType {
    PROJECTILE,
    MELEE
};

class Weapon : public Collider {
    public:
        Weapon(Properties& props, WeaponType weaponType);
        void Draw() override;
        void Clean() override;
        void Update(float dt) override;
        void UpdateColliders(const std::vector<Collider*>& colliders){m_Colliders = colliders;};
        WeaponType GetType() {return m_Type;}

        virtual ObjectType GetObjectType() override { return ObjectType::kWeapon; }
    private:
        WeaponType m_Type;
        bool m_auto_fire_enabled = false;
        std::vector<Collider*> m_Colliders;
};
