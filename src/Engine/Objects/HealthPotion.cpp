#include "HealthPotion.h"
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Health.h"

HealthPotion::HealthPotion(Properties& props, int health) : Collider(props) {
    m_health = health;
    m_animation = new Animation();
    m_animation->AddAnimation(
        "healthpotion",
        {m_texture_id, {0, 0, 16, 16}, 4, 300, SDL_FLIP_NONE, true});
    m_animation->SelectAnimation("healthpotion");
}

void HealthPotion::Update(float /*dt*/) {
    m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
    m_animation->Update();
}

void HealthPotion::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            dynamic_cast<Character*>(collidee)->GetHealth()->IncreaseHealth(
                GetHealth());
            m_marked_for_deletion = true;
            break;
        default:
            break;
    }
}
