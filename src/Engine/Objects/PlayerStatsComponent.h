#pragma once
#include <vector>
#include "Weapons/Weapon.h"
#include "Engine/Objects/Characters/Player.h"
#include "GridComponent.h"

class PlayerStatsComponent {
   public:
    PlayerStatsComponent(const PlayerStats& p_stats) : 
        m_playerStats(p_stats) {
            FillStatsVector();
    };

    void Draw();

    void Clean();

    void Update(const Player& player);

   private:
    std::vector<std::pair<std::string, std::variant<float, int>>> m_Stats;
    PlayerStats m_playerStats;
    void FillStatsVector();
};
