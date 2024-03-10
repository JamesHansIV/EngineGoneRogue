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
      m_Speed(speed),
      m_Angle(angle),
      m_PlayerOwned(playerOwned),
      m_Piercing(piercing),
      m_Owner(owner) {
    Vector2D const direction = Vector2D(cos(m_Angle), sin(m_Angle));
    Vector2D const velocity = direction * m_Speed;
    m_Velocity = velocity;
    m_RigidBody->SetVelocity(m_Velocity);
    m_Damage = damage;
}

void Projectile::Draw() {
    GameObject::Draw();
}

void Projectile::Update(float dt) {
    m_RigidBody->SetVelocity(m_Velocity);
    m_RigidBody->Update(dt);
    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);

    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    CheckOutOfBounds();
}

void Projectile::CheckOutOfBounds() {
    if (GetX() < 0.0F || GetY() < 0.0F ||
        GetX() + this->GetWidth() > Application::Get()->GetWindowWidth() ||
        GetY() + this->GetHeight() > Application::Get()->GetWindowHeight()) {
        m_MarkedForDeletion = true;
    }
}

void Projectile::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player: {
            Player* player = dynamic_cast<Player*>(collidee);
            if (!m_PlayerOwned && player->GetStateType() != StateType::Dodge &&
                !player->GetStats().GetDodgeInvincibility()) {
                m_MarkedForDeletion = true;
                //dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        }
        case ObjectType::Enemy:
            if (m_PlayerOwned) {
                // TODO: Piercing is broke, needs to be fixed
                m_MarkedForDeletion = true;
                AddNumberofEnemiesHit();
                double const life_steal_multiplier =
                    m_Owner->GetStats().GetLifeStealPercentage() / 100.0;
                int const increase_health_amount =
                    static_cast<float>(m_Damage) * life_steal_multiplier;
                m_Owner->GetHealth()->IncreaseHealth(increase_health_amount);
            }
            break;
        case ObjectType::MeleeWeapon:
        case ObjectType::Projectile:
            break;
        case ObjectType::Collider:
            m_MarkedForDeletion = true;
            break;
        default:
            break;
    }
}

void Projectile::Clean() {}
