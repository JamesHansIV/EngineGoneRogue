#include "HelixBullet.h"
#include <SDL2/SDL.h>
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

HelixBullet::HelixBullet(Properties& props, float speed, float angle,
                         bool playerOwned, bool flipped)
    : Projectile(props, speed, angle, playerOwned),
      m_flipped(static_cast<int>(flipped)),
      m_origin(Vector2D(GetX(), GetY())),
      m_t_pos(0, 0),
      m_wave_magnitude(1),
      m_scale(30.0F) {

    SetSpeed(speed / 25);
}

void HelixBullet::Draw() {
    Projectile::Draw();
}

void HelixBullet::Update(float /*dt*/) {
    m_t_pos.X += GetSpeed();
    m_t_pos.Y = m_flipped != 0 ? sin(m_t_pos.X + M_PI) : sin(m_t_pos.X);

    const float scaled_x = m_t_pos.X * m_scale;
    const float scaled_y = m_t_pos.Y * m_scale;

    const float rotated_x =
        scaled_x * cos(GetAngle()) - scaled_y * sin(GetAngle());
    const float rotated_y =
        scaled_x * sin(GetAngle()) + scaled_y * cos(GetAngle());

    SetX(rotated_x + m_origin.X);
    SetY(rotated_y + m_origin.Y);

    m_collision_box.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    CheckOutOfBounds();
}

void HelixBullet::OnCollide(Collider* collidee) {
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

void HelixBullet::Clean() {}
