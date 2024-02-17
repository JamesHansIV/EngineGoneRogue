#pragma once

#include "Engine/Events/EventManager.h"

class GameEventManager : public virtual EventManager {
   public:
    GameEventManager(Player* player, std::vector<GameObject*>& objects);
    GameEventManager(const GameEventManager&) = delete;
    GameEventManager(const GameEventManager&&) = delete;
    virtual GameEventManager& operator=(const GameEventManager&) = delete;

    void HandleEvents() override;
    void PushNewEvent(EventType event_type);

    void SetPlayer(Player* player) { m_player = player; }
    void ChestDrops(float chest_x, float chest_y);

   private:
    void HandleCustomEvents();
    void HandleSDLEvents();
    Player* m_player;
    std::vector<GameObject*>& m_Objects;
    float m_ChanceOfDrop = 0.1;
};
