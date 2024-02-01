#include "Projectile.h"
#include <SDL2/SDL.h>
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

Projectile::Projectile(Properties& props, float speed, float angle,
                       bool playerOwned, int damage)
    : Collider(props),
      m_Speed(speed),
      m_Angle(angle),
      m_PlayerOwned(playerOwned) {
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
        GetX() + this->GetWidth() > SCREEN_WIDTH ||
        GetY() + this->GetHeight() > SCREEN_HEIGHT) {
        m_MarkedForDeletion = true;
    }
}

void Projectile::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            if (!m_PlayerOwned) {
                m_MarkedForDeletion = true;
                //dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::Enemy:
            if (m_PlayerOwned) {
                m_MarkedForDeletion = true;
                SDL_Log("Damage: %d", m_Damage);
                dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(
                    m_Damage);
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
