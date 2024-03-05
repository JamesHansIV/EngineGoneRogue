#pragma once

#include "Engine/Events/EventManager.h"
#include "Engine/Events/ItemManager.h"

class GameEventManager : public virtual EventManager {
   public:
    GameEventManager(Player* player, std::vector<GameObject*>& objects);
    GameEventManager(const GameEventManager&) = delete;
    GameEventManager(const GameEventManager&&) = delete;
    virtual GameEventManager& operator=(const GameEventManager&) = delete;

    State* HandleEvents(ItemManager* ItemManager, State* GameState);

    void SetPlayer(Player* player) { m_player = player; }

   private:
    void HandleCustomEvents();
    void HandleSDLEvents();
    Player* m_player;
    std::vector<GameObject*>& m_Objects;
};
