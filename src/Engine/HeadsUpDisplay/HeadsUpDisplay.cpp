#include "Engine/HeadsUpDisplay/HeadsUpDisplay.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"

HUD::HUD(const Player& player)
    : m_weapon_inventory(player.GetPlayerWeapons()),
      m_item_inventory(player.GetPlayerItems()),
      m_experience_bar(100, 0),
      m_player_stats_component(player.GetStats()){};

void HUD::Draw(State& state) {
    // Timer
    TimerElement::Draw();
    m_weapon_inventory.Draw();
    m_experience_bar.Draw();
    m_item_inventory.Draw();

    if (state.GetType() == StateType::Pause) {
        PausedDraw();
    }
}

void HUD::PausedDraw() {
    // Timer
    m_player_stats_component.Draw();
}

void HUD::Update(const Player& player) {
    m_weapon_inventory.Update(player);
    m_experience_bar.Update(player);
    m_item_inventory.Update(player);
    m_player_stats_component.Update(player);
}
