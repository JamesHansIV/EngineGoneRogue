#include "RotatingBullet.h"
#include "Character.h"
#include "Engine/Renderer/Renderer.h"

RotatingBullet::RotatingBullet(Properties& props, float speed, float angle,
                               bool playerOwned, float rotationAngle,
                               float rotationSpeed, float radius)
    : Projectile(props, speed, angle, playerOwned),
      m_Center(GetX(), GetY()),
      m_RotationAngle(rotationAngle),
      m_RotationSpeed(rotationSpeed),
      m_Radius(radius) {}

void RotatingBullet::Draw() {
    Projectile::Draw();
}

void RotatingBullet::Update(float  /*dt*/) {
    m_Center = m_Center + GetVelocity();
    m_RotationAngle += m_RotationSpeed;

    const float next_x = m_Radius * cos(m_RotationAngle) + m_Center.X;
    const float next_y = m_Radius * sin(m_RotationAngle) + m_Center.Y;

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
            if (!PlayerOwned()) {
                MarkForDeletion();
                //dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::Enemy:
            if (PlayerOwned()) {
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
