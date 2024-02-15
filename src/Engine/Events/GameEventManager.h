#pragma once

#include "Engine/Events/EventManager.h"

class GameEventManager : public virtual EventManager {
   public:
    GameEventManager(Player& player, std::vector<GameObject*>& objects);
    GameEventManager(const GameEventManager&) = delete;
    GameEventManager(const GameEventManager&&) = delete;
    virtual GameEventManager& operator=(const GameEventManager&) = delete;

    void HandleEvents() override;
    void PushNewEvent(EventType event_type);

   private:
    void HandleCustomEvents();
    void HandleSDLEvents();
    Player& m_player;
    std::vector<GameObject*>& m_Objects;
};
