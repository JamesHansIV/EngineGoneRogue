#include "HealthPotion.h"
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Health.h"

HealthPotion::HealthPotion(Properties& props, int health) : Collider(props) {
    m_health = health;
}

void HealthPotion::Update(float /*dt*/) {
    m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
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
