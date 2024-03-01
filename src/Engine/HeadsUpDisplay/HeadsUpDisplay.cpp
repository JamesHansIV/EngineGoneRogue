#include "Engine/HeadsUpDisplay/HeadsUpDisplay.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"


HUD::HUD(const Player& player)
    : m_WeaponInventory(player.GetPlayerWeapons()), 
    m_ItemInventory(player.GetPlayerItems()), 
    m_ExperienceBar(100, 0), 
    m_PlayerStatsComponent(player.GetStats()){};

void HUD::Draw() {
    Renderer* renderer = Renderer::GetInstance();
    // Timer
    TimerElement::Draw();
    m_WeaponInventory.Draw();
    m_ExperienceBar.Draw();
    m_ItemInventory.Draw();
    m_PlayerStatsComponent.Draw();
}

void HUD::Update(const Player& player) {
    m_WeaponInventory.Update(player);
    m_ExperienceBar.Update(player);
    m_ItemInventory.Update(player);
    m_PlayerStatsComponent.Update(player);
}
