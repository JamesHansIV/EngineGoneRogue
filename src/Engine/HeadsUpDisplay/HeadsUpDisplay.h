#pragma once

#include "Engine/HeadsUpDisplay/TimerElement.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/ExperienceBar.h"
#include "Engine/Objects/ItemInventory.h"
#include "Engine/Objects/WeaponInventory.h"

class HUD {
   public:
    explicit HUD(const Player& player);

    void Draw();

    void Update(const Player& player);

   private:
    TimerElement m_TimerElement{};
    WeaponInventory m_WeaponInventory;
    ItemInventory m_ItemInventory;
    ExperienceBar m_ExperienceBar;
};
