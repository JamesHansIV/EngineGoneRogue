#pragma once

#include "Engine/HeadsUpDisplay/TimerElement.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/ExperienceBar.h"
#include "Engine/Objects/ItemInventory.h"
#include "Engine/Objects/PlayerStatsComponent.h"
#include "Engine/Objects/WeaponInventory.h"

class HUD {
   public:
    explicit HUD(const Player& player);

    void Draw(State& state);

    void Update(const Player& player);

   private:
    void PausedDraw();

    TimerElement m_TimerElement{};
    WeaponInventory m_WeaponInventory;
    ItemInventory m_ItemInventory;
    ExperienceBar m_ExperienceBar;
    PlayerStatsComponent m_PlayerStatsComponent;
};
