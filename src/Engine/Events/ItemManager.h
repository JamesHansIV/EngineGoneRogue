#pragma once

#include <vector>
#include "Engine/Events/EventManager.h"
#include "Engine/Objects/GameObject.h"

class ItemManager : public virtual EventManager {
    public:
        ItemManager(std::vector<GameObject*>& objects);
        void HandleEvent(Event* event); 
    private:
        std::vector<GameObject*>& m_Objects;
        float m_ChanceOfDrop = 0.2;

};