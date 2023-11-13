#pragma once
#include "GameObject.h"
#include "Engine/Input/InputChecker.h"
#include "Projectile.h"

/*
    notes:
    weapon type - enum?
    texture
*/

enum WeaponType {
    PROJECTILE,
    MELEE
};

class Weapon : public GameObject {
    public:
        Weapon(Properties& props, WeaponType weaponType) : GameObject(props), m_Type(weaponType){};
        void Draw() override;
        void Clean() override;
        void Update(float dt) override;
        void UpdateColliders(const std::vector<GameObject*>& colliders){m_Colliders = colliders;};
        WeaponType GetType() {return m_Type;};
    private:
        WeaponType m_Type;
        std::vector<GameObject*> m_Colliders;
};

