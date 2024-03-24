#include "MeleeWeapon.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Characters/Character.h"

MeleeWeapon::MeleeWeapon(Properties& props, MeleeWeaponStats& stats,
                         Player* owner, const std::string& name)
    : Weapon(props, stats, owner, name), m_stats(stats) {}

void MeleeWeapon::Draw() {
    Weapon::Draw();
}

void MeleeWeapon::Update(float dt) {

    if (m_flip == SDL_FLIP_VERTICAL) {
        SetX(GetX() - 12);
        m_collision_box.Set(GetX() - 12, GetY() - 12, GetHeight(), GetWidth());
    }

    Uint32 const time_since_last_attack = SDL_GetTicks() - m_last_attack_time;

    if ((InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT) ||
         (time_since_last_attack) < m_duration)) {
        float swing_angle = GetRotation();
        if (m_flip == SDL_FLIP_VERTICAL) {
            swing_angle -= 45;
        } else {
            swing_angle += 45;
        }
        const float radians = GetRadians();
        SetRotation(swing_angle);
        if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
            m_last_attack_time = SDL_GetTicks();
        }
        m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
        m_rigid_body->SetPosition(Vector2D{GetX(), GetY()});
        // move weapon
        Vector2D const direction = Vector2D(cos(radians), sin(radians));
        m_rigid_body->SetVelocity(direction * 10.0F);
        m_rigid_body->Update(dt);
        SetX(m_rigid_body->Position().X);
        SetY(m_rigid_body->Position().Y);

        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    } else {
        m_rigid_body->SetVelocity(Vector2D{0.0F, 0.0F});
        m_collision_box.Clear();
        //SetRotation(0.0);
    }
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
