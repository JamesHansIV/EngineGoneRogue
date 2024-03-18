#include "WeaponInventory.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/UI/GridComponent.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "SDL2/SDL_render.h"

const DrawColor kSelectedWeaponColor{182, 69, 153, 150};
const DrawColor kWeaponColor{0, 0, 0, 255};
const int kCellHeight = 40;
const int kCellWidth = 50;

void WeaponInventory::Draw() {
    GridComponent box_container(1, static_cast<int>(m_weapons.size()),0, 0, kCellHeight, kCellWidth);

    int column = 0;
    for (Weapon* weapon : m_weapons) {
        SDL_Rect src_rect;
        if (weapon->GetName() == "Sniper") {
            src_rect = {0, 0, 32, 16};
        } else {
            src_rect = {weapon->GetTilePos().col * weapon->GetTilePos().w,
                        weapon->GetTilePos().row * weapon->GetTilePos().h,
                        weapon->GetTilePos().w, weapon->GetTilePos().h};
        }

        Box* weapon_box = new BoxWithTexture(weapon->GetTextureID(), src_rect,
                                  weapon == m_selected_weapon ? kSelectedWeaponColor : kWeaponColor,
                                  weapon == m_selected_weapon);
        box_container.AddBox(weapon_box, 0, column);
        column++;
    }

    box_container.DrawGrid();
}

void WeaponInventory::Clean() {}

void WeaponInventory::Update(const Player& player) {
    m_weapons = player.GetPlayerWeapons();
    m_selected_weapon = player.GetCurrentWeapon();
}