#include "ItemManager.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/Chests/Chest.h"
#include "Engine/utils/utils.h"
#include <random>
#include <tuple>

ItemManager::ItemManager(std::vector<GameObject*>& objects)
    : m_Objects(objects) {}


void ItemManager::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();
    
     switch (e_type) {
        case EventType::PlaceChestIfNeededEvent:{
            auto* place_chestif_needed_event = dynamic_cast<PlaceChestIfNeededEvent*>(event);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(0.0F, 1.0F);
            float const random_number = dis(gen);
            SDL_Log("Rand: %f", random_number);
            if (random_number <= m_ChanceOfDrop) {
                auto* items = new std::vector<ItemType>{ItemType::Item1, ItemType::Item2, ItemType::Item3};
                Properties props13("", {1, 1, 18, 16}, 
                    {place_chestif_needed_event->GetX(), place_chestif_needed_event->GetY(), 32, 32}, 
                    0,"chest1");
                auto* chest1 = new Chest(props13, ChestType::Wooden, items);
                m_Objects.push_back(chest1);
                ColliderHandler::GetInstance()->AddCollider(chest1);
            }
            break;
        }
        case EventType::ChestOpenedEvent:{
            auto* chest_open_event = dynamic_cast<ChestOpenedEvent*>(event);
            for(auto entry: chest_open_event->GetItemTypes())
            {
                Properties props("", {0, 0, 16, 16}, {chest_open_event->GetIndex().first, 
                    chest_open_event->GetIndex().second, 32, 32}, 0, "");
                auto* item = new Item(props, entry);
                SDL_Log("Item texture: %s", item->GetTexture().c_str());
                m_Objects.push_back(item);
            }
            break;
        }
        default:
            break;
    }
}
