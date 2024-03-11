#include "Projectile.h"
#include <SDL2/SDL.h>
#include <cstddef>
#include "Engine/Application/Application.h"
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Renderer/Renderer.h"

Projectile::Projectile(Properties& props, float speed, float angle,
                       bool playerOwned, int damage, int piercing,
                       Player* owner)
    : Collider(props),
      m_speed(speed),
      m_angle(angle),
      m_player_owned(playerOwned),
      m_piercing(piercing),
      m_owner(owner) {
    Vector2D const direction = Vector2D(cos(m_angle), sin(m_angle));
    Vector2D const velocity = direction * m_speed;
    m_velocity = velocity;
    m_rigid_body->SetVelocity(m_velocity);
    m_damage = damage;
}

void Projectile::Draw() {
    GameObject::Draw();
}

void Projectile::Update(float dt) {
    m_rigid_body->SetVelocity(m_velocity);
    m_rigid_body->Update(dt);
    SetX(m_rigid_body->Position().X);
    SetY(m_rigid_body->Position().Y);

    m_collision_box.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    CheckOutOfBounds();
}

void Projectile::CheckOutOfBounds() {
    if (GetX() < 0.0F || GetY() < 0.0F ||
        GetX() + this->GetWidth() > Application::Get()->GetWindowWidth() ||
        GetY() + this->GetHeight() > Application::Get()->GetWindowHeight()) {
        m_marked_for_deletion = true;
    }
}

void Projectile::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player: {
            auto* player = dynamic_cast<Player*>(collidee);
            if (!m_player_owned && player->GetStateType() != StateType::Dodge &&
                (player->GetStats().GetDodgeInvincibility() == 0)) {
                m_marked_for_deletion = true;
                //dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        }
        case ObjectType::Enemy:
            if (m_player_owned) {
                // TODO: Piercing is broke, needs to be fixed
                m_marked_for_deletion = true;
                AddNumberofEnemiesHit();
                double const life_steal_multiplier =
                    m_owner->GetStats().GetLifeStealPercentage() / 100.0;
                int const increase_health_amount =
                    static_cast<float>(m_damage) * life_steal_multiplier;
                m_owner->GetHealth()->IncreaseHealth(increase_health_amount);
            }
            break;
        case ObjectType::MeleeWeapon:
        case ObjectType::Projectile:
            break;
        case ObjectType::Collider:
            m_marked_for_deletion = true;
            break;
        default:
            break;
    }
}

void Projectile::Clean() {}
