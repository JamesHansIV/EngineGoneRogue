#include "ItemManager.h"
#include <random>
#include <tuple>
#include "Apps/Game.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/Chests/Chest.h"
#include "Engine/Objects/HealthPotion.h"
#include "Engine/UI/ItemInventory.h"
#include "Engine/utils/utils.h"

ItemManager::ItemManager(Player* player) : m_player(player) {}

void ItemManager::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();
    switch (e_type) {
        case EventType::PlaceItemIfNeededEvent: {
            auto* place_item_if_needed_event =
                dynamic_cast<PlaceItemIfNeededEvent*>(event);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(0.0F, 1.0F);
            std::uniform_int_distribution<int> dis2(0, 9);

            float random_number = dis(gen);
            SDL_Log("Random number: %f", random_number);

            if (random_number <= m_chance_of_drop_chest) {
                auto* items = new std::vector<ItemType>();
                for (int i = 0; i < 3; ++i) {
                    int const random_index = dis2(gen);
                    auto const item = static_cast<ItemType>(random_index);
                    items->push_back(item);
                }
                Properties props13("", {1, 1, 18, 16},
                                   {place_item_if_needed_event->GetX(),
                                    place_item_if_needed_event->GetY(), 32, 32},
                                   0, "chest1");
                auto* chest1 = new Chest(props13, ChestType::Wooden, items);
                static_cast<Game*>(Application::Get())->AddObject(chest1);
                ColliderHandler::GetInstance()->AddCollider(chest1);
            }

            random_number = dis(gen);

            if (random_number <= m_chance_of_drop_health_potion) {
                Properties props11("weapons", {10, 1, 16, 16},
                                   {place_item_if_needed_event->GetX(),
                                    place_item_if_needed_event->GetY(), 25, 25},
                                   0, "healthpotion");
                auto* healthpotion = new HealthPotion(props11, 20);

                static_cast<Game*>(Application::Get())->AddObject(healthpotion);

                ColliderHandler::GetInstance()->AddCollider(healthpotion);
            }
            break;
        }
        case EventType::ChestOpenedEvent: {
            auto* chest_open_event = dynamic_cast<ChestOpenedEvent*>(event);
            for (auto entry : chest_open_event->GetItemTypes()) {
                Properties props("", {0, 0, 16, 16},
                                 {chest_open_event->GetIndex().first,
                                  chest_open_event->GetIndex().second, 32, 32},
                                 0, "");
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
                if (!added_to_inventory) {
                    delete item;
                }
            }
            break;
        }
        default:
            break;
    }
}
