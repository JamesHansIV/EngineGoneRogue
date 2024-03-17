#pragma once

#include <vector>
#include "Engine/Events/EventManager.h"
#include "Engine/Objects/GameObject.h"

class ItemManager : public virtual EventManager {
   public:
    ItemManager(std::vector<GameObject*>& objects, Player* player);
    void HandleEvent(Event* event);

   private:
    std::vector<GameObject*>& m_objects;
    float m_chance_of_drop = 0.9;
    Player* m_player;
};
