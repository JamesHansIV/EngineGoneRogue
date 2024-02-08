#include "MeleeWeapon.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Characters/Character.h"

MeleeWeapon::MeleeWeapon(Properties& props, MeleeWeaponStats& stats,
                         Player* owner)
    : Weapon(props, stats, owner) {}

void MeleeWeapon::Draw() {
    Weapon::Draw();
}

void MeleeWeapon::Update(float dt) {

    if (GetFlip() == SDL_FLIP_HORIZONTAL) {
        SetX(GetX() - 12);
        m_CollisionBox.Set(GetX() - 12, GetY() - 12, GetHeight(), GetWidth());
    }

    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        //float swing_angle = 0.0F;
        //if (GetFlip() == SDL_FLIP_HORIZONTAL) {
        //    swing_angle = -45.0F;
        //} else {
        //    swing_angle = 45.0F;
        //}
        // SetRotation(swing_angle);

        m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
        m_RigidBody->SetPosition(Vector2D{GetX(), GetY()});
        // move weapon
        Vector2D const direction =
            Vector2D(cos(GetRadians()), sin(GetRadians()));
        m_RigidBody->SetVelocity(direction * 50.0F);
        m_RigidBody->Update(dt);
        SetX(m_RigidBody->Position().X);
        SetY(m_RigidBody->Position().Y);

        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    } else {
        m_RigidBody->SetVelocity(Vector2D{0.0F, 0.0F});
        m_CollisionBox.clear();
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
        case ObjectType::MeleeWeapon:
        case ObjectType::Projectile:
        case ObjectType::Collider:
        default:
            break;
    }
}

void MeleeWeapon::Clean() {}
