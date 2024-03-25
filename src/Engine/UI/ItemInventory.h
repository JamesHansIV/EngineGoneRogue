#pragma once
#include <unordered_map>
#include <vector>
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Objects/Item.h"

class Player;

class ItemInventory {
   public:
    explicit ItemInventory(std::unordered_map<std::string, Item*> items)
        : m_items(std::move(items)){}; 

    void Draw();

    void Clean();

    void Update(const Player& player);

    void AddItem(Item& item);
    
   private:
    std::unordered_map<std::string, Item*> m_items;
};
