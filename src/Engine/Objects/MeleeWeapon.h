#pragma once

#include "Weapon.h"

class MeleeWeapon : public Weapon {
   public:
    MeleeWeapon(Properties& props, bool playerOwned);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override {
        return ObjectType::MeleeWeapon;
    }
};
