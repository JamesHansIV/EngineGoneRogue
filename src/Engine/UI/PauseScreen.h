#pragma once

#include "Engine/Objects/InventoryComponent.h"
#include "Engine/Objects/PlayerStatsComponent.h"
#include "Engine/Renderer/DrawElement.h"
#include "Engine/UI/Button.h"

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
