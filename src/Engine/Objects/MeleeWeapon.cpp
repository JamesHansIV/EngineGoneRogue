#include "MeleeWeapon.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Character.h"

MeleeWeapon::MeleeWeapon(Properties& props, bool playerOwned)
    : Weapon(props, playerOwned) {}

void MeleeWeapon::Draw() {
    GameObject::Draw();
}

void MeleeWeapon::Update(float  /*dt*/) {
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
        SetRotation(0.0);
    }
}

void MeleeWeapon::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            if (!PlayerOwned()) {
                dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::Enemy:
            if (PlayerOwned()) {
                dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::MeleeWeapon:
            break;
        case ObjectType::Projectile:
            break;
        case ObjectType::Collider:
            break;
        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
            break;
    }
}

void MeleeWeapon::Clean() {}