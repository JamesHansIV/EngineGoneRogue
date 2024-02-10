#include "HealthPotion.h"
#include "Engine/Objects/Health.h"
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/ColliderHandler.h"

HealthPotion::HealthPotion(Properties& props, int health ) : Collider(props) {
        m_health = health;
        m_Animation = new Animation();
        m_Animation->AddAnimation(
        "healthpotion",
        {m_TextureID, {0, 0, 16, 16},4, 15, SDL_FLIP_NONE, true});
        m_Animation->SelectAnimation("healthpotion");
}

void HealthPotion::Update(float dt) {
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
    m_Animation->Update();
}

void HealthPotion::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            dynamic_cast<Character*>(collidee)->GetHealth()->IncreaseHealth(GetHealth());
            m_MarkedForDeletion = true;
            ColliderHandler::GetInstance()->RemoveCollider(this);
            break;
        default:
            break;
    }
}
