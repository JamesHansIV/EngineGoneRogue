#pragma once

#include <vector>
#include "Engine/Events/EventManager.h"
#include "Engine/Objects/GameObject.h"

class ItemManager : public virtual EventManager {
   public:
    ItemManager(Player* player);
    void HandleEvent(Event* event);

   private:
    float m_chance_of_drop = 0.05;
    Player* m_player;
};
