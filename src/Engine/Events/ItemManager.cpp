#include "ItemManager.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/Chests/Chest.h"
#include "Engine/Objects/ItemInventory.h"
#include "Engine/utils/utils.h"
#include <random>
#include <tuple>

ItemManager::ItemManager(std::vector<GameObject*>& objects, Player* player)
    : m_objects(objects), m_player(player) {}


void ItemManager::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();
    
     switch (e_type) {
        case EventType::PlaceChestIfNeededEvent:{
            auto* place_chestif_needed_event = dynamic_cast<PlaceChestIfNeededEvent*>(event);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(0.0F, 1.0F);
            std::uniform_int_distribution<int> dis2(0, 8);

            float const random_number = dis(gen);

            if (random_number <= m_chance_of_drop) {
                auto* items = new std::vector<ItemType>();
                for (int i = 0; i < 3; ++i) {
                    int const random_index = dis2(gen);
                    auto const item = static_cast<ItemType>(random_index);
                    items->push_back(item);
                }
                Properties props13("", {1, 1, 18, 16}, 
                    {place_chestif_needed_event->GetX(), place_chestif_needed_event->GetY(), 32, 32}, 
                    0,"chest1");
                auto* chest1 = new Chest(props13, ChestType::Wooden, items);
                m_objects.push_back(chest1);
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
                auto it = m_player->GetPlayerItems().find(item->GetName());
                bool added_to_inventory = false;
                if (it == m_player->GetPlayerItems().end()) {
                    m_player->AddItem(item);
                    added_to_inventory = true;
                } else {
                    (*it).second->AddCount();
                }
                item->Apply(m_player);
                if(!added_to_inventory){
                    delete item;
                }
            }
            break;
        }
        default:
            break;
    }
}

