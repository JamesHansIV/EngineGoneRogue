#include "Engine/HeadsUpDisplay/HeadsUpDisplay.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"

HUD::HUD(const Player& player)
    : m_WeaponInventory(player.GetPlayerWeapons()), m_ExperienceBar(100, 0) {}

void HUD::Draw() {
    Renderer* renderer = Renderer::GetInstance();
    // Timer
    TimerElement::Draw();
    m_WeaponInventory.Draw();
    m_ExperienceBar.Draw();
}

void HUD::Update(const Player& player) {
    m_WeaponInventory.Update(player);
    m_ExperienceBar.Update(player);
}
