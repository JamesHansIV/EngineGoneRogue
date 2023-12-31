#include "WaveBullet.h"
#include <SDL2/SDL.h>
#include "Character.h"
#include "Enemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

WaveBullet::WaveBullet(Properties& props, float speed, float angle,
                       bool playerOwned, bool flipped)
    : Projectile(props, speed, angle, playerOwned),
      m_Flipped(flipped),
      m_Origin(Vector2D(GetX(), GetY())),
      m_TPos(0, 0),
      m_WaveMagnitude(1),
      m_Scale(30.0F) {

    SetSpeed(speed / 25);
}

void WaveBullet::Draw() {
    GameObject::Draw();
}

void WaveBullet::Update(float dt) {
    m_TPos.X += GetSpeed();
    m_TPos.Y = m_Flipped ? sin(m_TPos.X + M_PI) : sin(m_TPos.X);

    const float scaled_x = m_TPos.X * m_Scale;
    const float scaled_y = m_TPos.Y * m_Scale;

    const float rotated_x =
        scaled_x * cos(GetAngle()) - scaled_y * sin(GetAngle());
    const float rotated_y =
        scaled_x * sin(GetAngle()) + scaled_y * cos(GetAngle());

    SetX(rotated_x + m_Origin.X);
    SetY(rotated_y + m_Origin.Y);

    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    CheckOutOfBounds();
}

void WaveBullet::OnCollide(Collider* collidee) {
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

void WaveBullet::Clean() {}
