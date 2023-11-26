#pragma once
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Collider.h"
#include "GameObject.h"
#include "Projectile.h"

class Weapon : public Collider {
   public:
    Weapon(Properties& props, bool playerOwned = false)
        : Collider(props), m_PlayerOwned(playerOwned) {}

    virtual void Draw() override = 0;
    virtual void Clean() override = 0;
    virtual void Update(float dt) override = 0;

    inline bool PlayerOwned() { return m_PlayerOwned; }

   private:
    bool m_PlayerOwned;
    bool m_auto_fire_enabled = false;
};
