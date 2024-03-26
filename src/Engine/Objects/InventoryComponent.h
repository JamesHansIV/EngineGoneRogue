#pragma once
#include <unordered_map>
#include <vector>
#include "Engine/Objects/ItemInventory.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Item.h"

class Player;

class InventoryComponent {
   public:
    InventoryComponent() = default;
    explicit InventoryComponent(std::unordered_map<std::string, Item*> items)
        : m_items(std::move(items)){};

    void SetItems(std::unordered_map<std::string, Item*> items) {
        m_items = items;
    }

    void Draw();

    void Clean();

    void Update(const Player& player);

   private:
    std::unordered_map<std::string, Item*> m_items;
};
