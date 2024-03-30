#pragma once

#include "Engine/Events/EventManager.h"
#include "Engine/Events/ItemManager.h"

class GameEventManager : public virtual EventManager {
   public:
    GameEventManager(Player* player);
    GameEventManager(const GameEventManager&) = delete;
    GameEventManager(const GameEventManager&&) = delete;
    virtual GameEventManager& operator=(const GameEventManager&) = delete;

    void HandleEvents(ItemManager* ItemManager, State* GameState);

    void SetPlayer(Player* player) { m_player = player; }

   private:
    void HandleCustomEvents(const SDL_Event& event, ItemManager* ItemManager,
                            State* GameState);
    void HandleSDLEvents();
    Player* m_player;
};
