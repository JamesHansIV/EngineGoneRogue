#include "RotatingBullet.h"
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Renderer/Renderer.h"

RotatingBullet::RotatingBullet(Properties& props, float speed, float angle,
                               bool playerOwned, float rotationAngle,
                               float rotationSpeed, float radius)
    : Projectile(props, speed, angle, playerOwned),
      m_center(GetX(), GetY()),
      m_rotation_angle(rotationAngle),
      m_rotation_speed(rotationSpeed),
      m_radius(radius) {}

void RotatingBullet::Draw() {
    Projectile::Draw();
}

void RotatingBullet::Update(float /*dt*/) {
    m_center = m_center + GetVelocity();
    m_rotation_angle += m_rotation_speed;

    const float next_x = m_radius * cos(m_rotation_angle) + m_center.X;
    const float next_y = m_radius * sin(m_rotation_angle) + m_center.Y;

    SetX(next_x);
    SetY(next_y);

    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    CheckOutOfBounds();
}

void RotatingBullet::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            if (!IsPlayerOwned()) {
                MarkForDeletion();
                //dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::Enemy:
            if (IsPlayerOwned()) {
                MarkForDeletion();
                dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::MeleeWeapon:
        case ObjectType::Collider:
            MarkForDeletion();
            break;
        default:
            break;
    }
}

void RotatingBullet::Clean() {}
