#include "PlayerStatsComponent.h"
#include <iomanip>
#include <sstream>
#include "Engine/Objects/GridComponent.h"
const DrawColor kBoxColor = {0, 0, 0, 255};
const int kCellWidth = 200;
const int kCellHeight = 30;

void PlayerStatsComponent::Draw() {
    int row = 0;

    GridComponent stats_grid(static_cast<int>(m_Stats.size()), 2, 0, 80,
                             kCellHeight, kCellWidth);
    stats_grid.SetColumnWidth(0, 230);
    stats_grid.SetColumnWidth(1, 150);
  
    for (const auto& stat : m_Stats) {
        SDL_Rect src_rect = {0, 0, 16, 16};
        auto* box1 = new BoxWithText(stat.first, src_rect, kBoxColor, false, TextAlign::kLeft);
        BoxWithText* box2 = nullptr;
        if (stat.first == "Speed" || stat.first == "DodgeSpeed") {
            float value = std::get<float>(stat.second);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2)
               << value;  // Set precision to 2
            box2 = new BoxWithText(ss.str(), src_rect, kBoxColor, false, TextAlign::kCenter);
        } else {
            box2 = new BoxWithText(std::to_string(std::get<int>(stat.second)),
                                   src_rect, kBoxColor, false, TextAlign::kCenter);
        }

        stats_grid.AddBox(box1, row, 0);
        stats_grid.AddBox(box2, row, 1);
        row += 1;
    }

    stats_grid.DrawGrid();
}

void PlayerStatsComponent::Update(const Player& player) {
    m_playerStats = player.GetStats();
    m_Stats.clear();
    FillStatsVector();
}

void PlayerStatsComponent::FillStatsVector() {
    m_Stats.push_back(std::make_pair("Speed", m_playerStats.GetSpeed()));
    m_Stats.push_back(std::make_pair("DodgeCD", m_playerStats.GetDodgeCd()));
    m_Stats.push_back(
        std::make_pair("DodgeSpeed", m_playerStats.GetDodgeSpeed()));
    m_Stats.push_back(
        std::make_pair("RangedDamage", m_playerStats.GetRangedDamage()));
    m_Stats.push_back(
        std::make_pair("MeleeDamage", m_playerStats.GetMeleeDamage()));
    m_Stats.push_back(
        std::make_pair("Experience", m_playerStats.GetExperience()));
    m_Stats.push_back(std::make_pair("Piercing", m_playerStats.GetPiercing()));
    m_Stats.push_back(
        std::make_pair("ArmorPercentage", m_playerStats.GetArmorPercentage()));
    m_Stats.push_back(
        std::make_pair("HPRegenRate", m_playerStats.GetHPRegenRate()));
    m_Stats.push_back(std::make_pair("Level", m_playerStats.GetLevel()));
    m_Stats.push_back(std::make_pair("LifeStealPercentage",
                                     m_playerStats.GetLifeStealPercentage()));
    m_Stats.push_back(
        std::make_pair("KillCount", m_playerStats.GetKillCount()));
}
