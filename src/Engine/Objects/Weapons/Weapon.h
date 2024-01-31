#pragma once
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Projectiles/Projectile.h"

class Weapon : public Collider {
   public:
    explicit Weapon(Properties& props, bool playerOwned = false)
        : Collider(props), m_PlayerOwned(playerOwned) {}

    void Draw() override { GameObject::Draw(); }

    virtual void Clean() override = 0;
    virtual void Update(float dt) override = 0;

    inline bool PlayerOwned() { return m_PlayerOwned; }

   private:
    bool m_PlayerOwned;
    bool m_auto_fire_enabled = false;
};
