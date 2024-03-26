#pragma once

#include "Engine/HeadsUpDisplay/TimerElement.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/ExperienceBar.h"
#include "Engine/Objects/InventoryComponent.h"
#include "Engine/Objects/ItemInventory.h"
#include "Engine/Objects/PlayerStatsComponent.h"
#include "Engine/Objects/WeaponInventory.h"

class HUD {
   public:
    explicit HUD(const Player& player);

    void Reset(const Player& player);

    void Draw(State& state);

    void Update(const Player& player);

   private:
    void PausedDraw();

    TimerElement m_timer_element{};
    WeaponInventory m_weapon_inventory;
    ItemInventory m_item_inventory;
    ExperienceBar m_experience_bar;
};
