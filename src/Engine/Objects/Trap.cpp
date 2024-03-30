#include "Trap.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"

Trap::Trap(Properties& props, int damage) : Collider(props){
    m_damage = damage;
    m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
    m_animation = new Animation();
    m_animation->AddAnimation(
        "trap_idle",
        {"trap_idle", {0, 0, 16, 16}, 1, 0, SDL_FLIP_NONE, true});
    m_animation->AddAnimation(
        "trap_opening", {"trap_opening", {0, 0, 16, 16}, 5, 50, SDL_FLIP_NONE, false});
    m_animation->AddAnimation(
            "trap_closing", {"trap_closing", {0, 0, 16, 16}, 3, 50, SDL_FLIP_NONE, false});
    m_animation->SelectAnimation("trap_idle");
    m_state = TrapState::IDLE;
}

void Trap::Update(float dt) {
    m_animation->Update();

    if (m_state == TrapState::TOUCHEDBYPLAYER) {
        SetAnimationAndState("trap_opening", TrapState::OPENING);
    } else if (m_state == TrapState::OPENING && m_animation->Ended()) {
        if (m_colliding_with_player) {
            m_state = TrapState::OPENED;
        } else {
            SetAnimationAndState("trap_closing", TrapState::CLOSING);
        }
    } else if (m_state == TrapState::CLOSING && m_animation->Ended()) {
        SetAnimationAndState("trap_idle", TrapState::IDLE);
    } else if (!m_colliding_with_player && m_animation->Ended() && m_state != TrapState::CLOSING) {
        SetAnimationAndState("trap_idle", TrapState::IDLE);
    }

    m_colliding_with_player = false;
}

void Trap::SetAnimationAndState(const std::string& animationName, TrapState state) {
    m_animation->SelectAnimation(animationName);
    m_state = state;
}

void Trap::OnCollide(Collider *collidee){
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:
            break;
        case ObjectType::Player:
            m_colliding_with_player = true;
            if(m_state == TrapState::IDLE){
                m_state = TrapState::TOUCHEDBYPLAYER;
            }
        case ObjectType::Enemy:
            UnCollide(collidee);
            break;
        default:
            UnCollide(collidee);
            break;
    }
}