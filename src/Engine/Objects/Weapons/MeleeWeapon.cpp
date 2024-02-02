#include "MeleeWeapon.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Characters/Character.h"

MeleeWeapon::MeleeWeapon(Properties& props, MeleeWeaponStats& stats)
    : Weapon(props, stats) {}

void MeleeWeapon::Draw() {
    Weapon::Draw();
}

void MeleeWeapon::Update(float /*dt*/) {
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());

    if (GetFlip() == SDL_FLIP_HORIZONTAL) {
        SetX(GetX() - 12);
        m_CollisionBox.Set(GetX() - 12, GetY() - 12, GetHeight(), GetWidth());
    }

    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        float swing_angle = 0.0F;
        if (GetFlip() == SDL_FLIP_HORIZONTAL) {
            swing_angle = -45.0F;
        } else {
            swing_angle = 45.0F;
        }
        SetRotation(swing_angle);
        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    } else {
        //SetRotation(0.0);
    }
    SDL_Log("sword angle: %f", GetRotation());
}

void MeleeWeapon::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            if (!IsPlayerOwned()) {
                dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::Enemy:
            if (IsPlayerOwned()) {
                dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::MeleeWeapon:
        case ObjectType::Projectile:
        case ObjectType::Collider:
            break;
        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
            break;
    }
}

void MeleeWeapon::Clean() {}
