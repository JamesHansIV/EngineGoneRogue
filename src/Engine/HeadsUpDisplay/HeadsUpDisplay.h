#pragma once

#include "Engine/HeadsUpDisplay/TimerElement.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/UI/ExperienceBar.h"
#include "Engine/UI/InventoryComponent.h"
#include "Engine/UI/ItemInventory.h"
#include "Engine/UI/PlayerStatsComponent.h"
#include "Engine/UI/WeaponInventory.h"

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
