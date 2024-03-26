#pragma once

#include "Engine/UI/Button.h"
#include "Engine/UI/InventoryComponent.h"
#include "Engine/UI/PlayerStatsComponent.h"

class PauseScreen {
   public:
    PauseScreen() = default;
    PauseScreen(const Player& player);

    void Draw();
    void Update(const Player& player);
    State* HandleEvent(Event* event);

   private:
    PlayerStatsComponent m_player_stats;
    InventoryComponent m_inventory;
    Button m_button;
};
