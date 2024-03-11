#include "Chest.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Events/Event.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Item.h"
#include "Engine/utils/utils.h"
#include "SDL2/SDL_log.h"
constexpr float kCloseDistance = 45.0F; 

Chest::Chest(Properties& props, ChestType ctype, std::vector<ItemType>* chestItems) : Collider(props) 
{   
    m_chest_items = chestItems;
    m_chest_type = ctype;
    m_Animation = new Animation();

    if (m_chest_type == ChestType::Wooden) {
        m_idle_texture = "wooden-chest-idle";
        m_opening_texture = "wooden-chest-opening";
    } else if (m_chest_type == ChestType::Silver) {
        m_idle_texture = "silver-chest-idle";
        m_opening_texture = "silver-chest-opening";
    } else if (m_chest_type == ChestType::Golden) {
        m_idle_texture = "golden-chest-idle";
        m_opening_texture = "golden-chest-opening";
    }

    m_Animation->AddAnimation(
        m_idle_texture,
        {m_idle_texture, {0, 0, 18, 16}, 1, 15, SDL_FLIP_NONE, true});

    m_Animation->AddAnimation(
        m_opening_texture,
        {m_opening_texture, {0, 0, 18, 16}, 5, 300 , SDL_FLIP_NONE, true});

    m_Animation->SelectAnimation(m_idle_texture);
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
}

void Chest::Update(float  /*dt*/) {
    m_Animation->Update();
  
    if (m_Animation->GetAnimationID() == m_opening_texture && m_Animation->GetCurrentFrame() == 4) {
        MarkForDeletion();
        ColliderHandler::GetInstance()->RemoveCollider(this);
        auto *index = new std::pair<float,float>(this->GetMidPointX(), this->GetMidPointY());
        PushNewEvent(EventType::ChestOpenedEvent, m_chest_items, index);
    }
}

void Chest::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }
    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
          if(m_Animation->GetAnimationID() != m_opening_texture){
            m_Animation->SelectAnimation(m_opening_texture); 
          }
            break;
        default:
            break;
    }
}
