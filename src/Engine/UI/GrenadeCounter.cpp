#include "GrenadeCounter.h"
#include <string>
#include "Engine/Objects/Characters/Player.h"
#include "Engine/UI/GridComponent.h"

const DrawColor kColor{0, 0, 0, 255};
const int kCellHeight = 40;
const int kCellWidth = 50;

void GrenadeCounter::Update(const Player &player){
    m_grenade_count = player.GetBombCount();
}

void GrenadeCounter::Draw(){
    GridComponent box_container(1, 1, 0, 40, kCellHeight, kCellWidth);

    auto* grenade_texture = new BoxWithCounter(m_grenade_count, "weapons", {3*16, 10*16, 16, 16}, kColor, true);
    auto* grenade_count = new BoxWithText(std::to_string(m_grenade_count), {0,0,16,16}, kColor, false, TextAlign::KCenter);
    
    box_container.AddBox(grenade_texture, 0, 0);
    // box_container.AddBox(grenade_count, 0, 1);

    box_container.DrawGrid();
}