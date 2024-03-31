#include "DestructibleItem.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Objects/Weapons/MeleeWeapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/State/State.h"
#include "Engine/utils/utils.h"

DestructibleItem::DestructibleItem(Properties& props) : Collider(props) {
    m_animation = new Animation();
    m_animation->AddAnimation("item_idle", {"destructible_objects", {0,0, 32, 32}, 1, 0, SDL_FLIP_NONE, false});
    m_animation->AddAnimation("item_destroying", {"destructible_objects", {0,0, 32, 32}, 7, 150, SDL_FLIP_NONE, false});
    m_animation->AddAnimation("item_destroyed", {"destructible_objects", {0,6, 32, 32}, 1, 0, SDL_FLIP_NONE, false});
    m_animation->SelectAnimation("item_idle");
    m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
}

DestructibleItem::DestructibleItem(Collider* collider) :Collider(collider){
    m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
    m_animation->SelectAnimation("item_idle");
}

void DestructibleItem::Update(float dt){
    m_animation->Update();
    if(m_animation->GetAnimationID() == "item_destroying" && m_animation->Ended()){
        m_animation->SelectAnimation("item_destroyed");
    }
    if(m_animation->GetAnimationID() == "item_destroyed" && m_animation->Ended()){
        auto* index = new std::pair<float, float>(this->GetMidPointX(),
                                                  this->GetMidPointY());
        PushNewEvent(EventType::ItemDestroyedEvent, index);
        m_marked_for_deletion = true;
    }
    m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
}

void DestructibleItem::OnCollide(Collider *collidee){
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:{
            auto* projectile = dynamic_cast<Projectile*>(collidee);
            if(projectile->IsPlayerOwned()){
                if(m_animation->GetAnimationID() == "item_idle"){
                    m_animation->SelectAnimation("item_destroying");
                }
            }
            break;
        }
        case ObjectType::MeleeWeapon:{
            auto* melee = dynamic_cast<MeleeWeapon*>(collidee);
            if(melee->IsPlayerOwned()){
                if(m_animation->GetAnimationID() == "item_idle"){
                    m_animation->SelectAnimation("item_destroying");
                }
            }
            break;
        }
        case ObjectType::Player:
            break;
        case ObjectType::Enemy:
            UnCollide(collidee);
            break;
        default:
            break;
    }
}