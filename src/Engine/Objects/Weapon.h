#pragma once
#include "GameObject.h"
#include "Engine/Input/InputChecker.h"
#include "Projectile.h"
#include "Engine/Objects/Collider.h"


class Weapon : public Collider {
    public:
        Weapon(Properties& props, bool playerOwned = false) : Collider(props), m_PlayerOwned(playerOwned) {}
        virtual void Draw() override = 0;
        virtual void Clean() override = 0;
        virtual void Update(float dt) override = 0;

        inline bool GetPlayerOwned() { return m_PlayerOwned; }

    private:
        bool m_PlayerOwned;
        bool m_auto_fire_enabled = false;
};
