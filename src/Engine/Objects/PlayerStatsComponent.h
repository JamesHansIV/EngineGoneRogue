#pragma once
#include <vector>
#include "Weapons/Weapon.h"
#include "Engine/Objects/Characters/Player.h"

class PlayerStatsComponent {
   public:
    explicit PlayerStatsComponent(const PlayerStats& p_stats) : 
        m_player_stats(p_stats) {
            FillStatsVector();
    };

    void Draw();

    void Clean();

    void Update(const Player& player);

   private:
    std::vector<std::pair<std::string, std::variant<float, int>>> m_stats;
    PlayerStats m_player_stats;
    void FillStatsVector();
};
