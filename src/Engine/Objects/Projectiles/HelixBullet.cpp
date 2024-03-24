#include "HelixBullet.h"
#include <SDL2/SDL.h>
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

HelixBullet::HelixBullet(Properties& props, float speed, float angle,
                         AnimationInfo hitAnimationInfo, bool playerOwned,
                         bool flipped)
    : Projectile(props, speed, angle, hitAnimationInfo, playerOwned),
      m_flipped(static_cast<int>(flipped)),
      m_origin(Vector2D(GetX(), GetY())),
      m_t_pos(0, 0) {

    SetSpeed(speed / 25);
}

void HelixBullet::Draw() {
    Projectile::Draw();
}

void HelixBullet::Update(float /*dt*/) {
    if (Hit()) {
        m_animation->Update();
        if (m_animation && m_animation->Ended()) {
            m_marked_for_deletion = true;
        }
        return;
    }
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
    Projectile::OnCollide(collidee);
}

void HelixBullet::Clean() {}
