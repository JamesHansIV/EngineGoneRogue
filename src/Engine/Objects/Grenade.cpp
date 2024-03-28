#include "Engine/Objects/Grenade.h"

Grenade::Grenade(Properties& props, GrenadeStats stats) : Collider(props) {

    m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
    m_stats = stats;
    m_animation = new Animation();
    m_animation->AddAnimation(
        "live_grenade",
        {"weapons", {10, 3, 16, 16}, 2, 200, SDL_FLIP_NONE, true});
    m_animation->AddAnimation(
        "explosion", {"weapons", {6, 0, 32, 32}, 6, 100, SDL_FLIP_NONE, false});

    m_animation->SelectAnimation("live_grenade");
    m_state = BombState::INACTIVE;
}

void Grenade::Update(float dt) {
    m_rigid_body->Update(dt);
    SetX(m_rigid_body->Position().X);
    SetY(m_rigid_body->Position().Y);

    m_collision_box.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_animation->Update();
    m_time_alive += dt;

    // Make the grenade flash faster as it gets closer to exploding
    if (m_time_alive >= m_time_to_increase_animation_speed) {
        m_animation->SetAnimationSpeed(m_animation->GetAnimationSpeed() - 30);
        m_time_to_increase_animation_speed += 0.5;
    }

    if (m_state == BombState::EXPLODING_DAMAGING) {
        m_state = BombState::EXPLODING;
    }

    if (m_state == BombState::INACTIVE && m_time_alive > .1) {
        m_state = BombState::ACTIVE;
    }

    if (m_state == BombState::ACTIVE &&
        m_time_alive >= m_stats.time_to_explode) {
        m_animation->SelectAnimation("explosion");
        // TODO: center explosion after increasing size
        m_dst_rect = {GetX() - GetWidth(), GetY() - GetHeight(), GetWidth() * 4,
                      GetHeight() * 4};
        m_rigid_body->SetPosition(Vector2D{m_dst_rect.x, m_dst_rect.y});
        m_rigid_body->SetVelocity(Vector2D{.1, 0});
        m_collision_box.Set(m_dst_rect.x, m_dst_rect.y, m_dst_rect.h,
                            m_dst_rect.w);
        m_state = BombState::EXPLODING_DAMAGING;
        m_time_to_increase_animation_speed = 99999;
    }

    if (m_state == BombState::EXPLODING && m_animation->Ended()) {
        m_marked_for_deletion = true;
    }
}

void Grenade::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    if (m_state == BombState::EXPLODING) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:
            break;
        case ObjectType::Player:
        case ObjectType::Enemy:
            m_rigid_body->SetVelocity((m_rigid_body->Position() -
                                       collidee->GetRigidBody()->Position()) *
                                      (2.0 / 10));
            UnCollide(collidee);
            break;
        default:
            UnCollide(collidee);
            break;
    }
}
