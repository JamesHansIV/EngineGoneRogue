#include "Projectile.h"
#include <SDL2/SDL.h>
#include <cstddef>
#include "Engine/Application/Application.h"
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/utils/utils.h"

Projectile::Projectile(Properties& props, float speed, float angle,
                       AnimationInfo hitAnimationInfo, bool playerOwned,
                       int damage, int piercing, Player* owner)
    : Collider(props),
      m_speed(speed),
      m_angle(angle),
      m_player_owned(playerOwned),
      m_piercing(piercing),
      m_owner(owner),
      m_hit(false),
      m_hit_animation_info(hitAnimationInfo) {
    Vector2D const direction = Vector2D(cos(m_angle), sin(m_angle));
    Vector2D const velocity = direction * m_speed;
    m_velocity = velocity;
    m_rigid_body->SetVelocity(m_velocity);
    m_damage = damage;
    m_animation = new Animation();
    m_animation->AddAnimation(m_hit_animation_info.TextureID,
                              m_hit_animation_info);
}

void Projectile::Draw() {
    if (m_hit) {
        const Rect dstRect = {GetX(), GetY(), GetWidth() * 3, GetHeight() * 3};
        m_animation->Draw(dstRect);
        return;
    }
    GameObject::DrawRect();
}

void Projectile::Update(float dt) {
    if (m_hit) {
        m_animation->Update();
        m_collision_box.Clear();
        if (m_animation && m_animation->Ended()) {
            m_marked_for_deletion = true;
        }
        return;
    }
    m_rigid_body->SetVelocity(m_velocity);
    m_rigid_body->Update(dt);
    SetX(m_rigid_body->Position().X);
    SetY(m_rigid_body->Position().Y);

    m_collision_box.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    CheckOutOfBounds();
}

void Projectile::CheckOutOfBounds() {
    if (GetX() < 0.0F || GetY() < 0.0F ||
        GetX() + this->GetWidth() > LevelWidth ||
        GetY() + this->GetHeight() > LevelHeight) {
        m_marked_for_deletion = true;
    }
}

void Projectile::HitTarget() {
    m_hit = true;
    m_animation->SelectAnimation(m_hit_animation_info.TextureID);
    SetX(GetX() + m_rigid_body->Velocity().X);
    SetY(GetY() + m_rigid_body->Velocity().Y);
}

void Projectile::CollideWithEnemy(){
    AddNumberofEnemiesHit();
    if(m_piercing + 1 == m_numberof_enemies_hit){
        HitTarget();
    }
    double const life_steal_multiplier =
        m_owner->GetStats().GetLifeStealPercentage() / 100.0;
    int const increase_health_amount =
        static_cast<float>(m_damage) * life_steal_multiplier;
    m_owner->GetHealth()->IncreaseHealth(increase_health_amount);
    Application::Get()->GetAudioManager().PlaySound("low", 20, 0);
}

void Projectile::OnCollide(Collider* collidee) {
    if (this == collidee || m_hit) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player: {
            auto* player = dynamic_cast<Player*>(collidee);
            if (!m_player_owned && player->GetStateType() != StateType::Dodge &&
                (player->GetStats().GetDodgeInvincibility() == 0)) {
                HitTarget();
                //dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
                Application::Get()->GetAudioManager().PlaySound("high", 5, 0);
            }
            break;
        }
        case ObjectType::Enemy:
                // TODO: Piercing is broke, needs to be fixed
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
