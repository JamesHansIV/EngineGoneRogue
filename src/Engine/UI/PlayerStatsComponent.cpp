#include "PlayerStatsComponent.h"
#include <iomanip>
#include <sstream>
#include "Engine/Events/Event.h"
#include "Engine/UI/GridComponent.h"
const DrawColor kBoxColor = {0, 0, 0, 255};
const DrawColor kBorderColor = {255,255,255,255};
const int kCellWidth = 200;
const int kCellHeight = 30;

void PlayerStatsComponent::Draw() {
    int row = 0;

    GridComponent stats_grid(static_cast<int>(m_stats.size()), 2, 0, 130,
                             kCellHeight, kCellWidth);
    stats_grid.SetColumnWidth(0, 230);
    stats_grid.SetColumnWidth(1, 100);
    SDL_Rect const src_rect = {0, 0, 16, 16};
  
    for (const auto& stat : m_stats) {

        auto* box1 = new BoxWithText(stat.first, src_rect, kBoxColor, false, TextAlign::KLeft);
        BoxWithText* box2 = nullptr;
        if (stat.first == "Speed" || stat.first == "DodgeSpeed") {
            float const value = std::get<float>(stat.second);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2)
               << value;  // Set precision to 2
            box2 = new BoxWithText(ss.str(), src_rect, kBoxColor, false, TextAlign::KCenter);
        } else {
            box2 = new BoxWithText(std::to_string(std::get<int>(stat.second)),
                                   src_rect, kBoxColor, false, TextAlign::KCenter);
        }

        stats_grid.AddBox(box1, row, 0);
        stats_grid.AddBox(box2, row, 1);
        row += 1;
    }
    stats_grid.SetGridBorderColor(kBorderColor);
    stats_grid.DrawGrid();

    GridComponent box_container2(1, 1, 0, 100, kCellHeight, kCellWidth);
    auto* header = new BoxWithText("Player Stats", src_rect, kBoxColor, false, KCenter);
    box_container2.AddBox(header, 0, 0);
    int const total_width = stats_grid.GetColumnWidth(0) + stats_grid.GetColumnWidth(1); 
    box_container2.SetColumnWidth(0, total_width);
    box_container2.SetGridBorderColor(kBorderColor);
    box_container2.DrawGrid();
    
}

void PlayerStatsComponent::Update(const Player& player) {
    m_player_stats = player.GetStats();
    m_stats.clear();
    FillStatsVector();
}

void PlayerStatsComponent::FillStatsVector() {
    m_stats.emplace_back("Speed", m_player_stats.GetSpeed());
    m_stats.emplace_back("DodgeCD", m_player_stats.GetDodgeCd());
    m_stats.emplace_back("DodgeSpeed", m_player_stats.GetDodgeSpeed());
    m_stats.emplace_back("RangedDamage", m_player_stats.GetRangedDamage());
    m_stats.emplace_back("MeleeDamage", m_player_stats.GetMeleeDamage());
    m_stats.emplace_back("Experience", m_player_stats.GetExperience());
    m_stats.emplace_back("Piercing", m_player_stats.GetPiercing());
    m_stats.emplace_back("ArmorPercentage", m_player_stats.GetArmorPercentage());
    m_stats.emplace_back("HPRegenRate", m_player_stats.GetHPRegenRate());
    m_stats.emplace_back("Level", m_player_stats.GetLevel());
    m_stats.emplace_back("LifeStealPercentage",
                                     m_player_stats.GetLifeStealPercentage());
    m_stats.emplace_back("KillCount", m_player_stats.GetKillCount());
}
