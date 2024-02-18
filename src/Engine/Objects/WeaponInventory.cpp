#include "WeaponInventory.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "SDL2/SDL_render.h"
#include "Engine/Objects/BoxContainer.h"

const DrawColor kSelectedWeaponColor{182, 69, 153, 150};

void WeaponInventory::Draw() {
    Renderer* renderer = Renderer::GetInstance();
    
    BoxContainer box_container(0, 0, 40, 50, 0);

    for (Weapon* weapon : m_Weapons) {
        // if (m_SelectedWeapon == weapon) {
        //     SDL_Rect bg_rect = {renderer->GetCameraX(),
        //                         renderer->GetCameraY(), 50, 40};
        //     renderer->DrawRect(bg_rect, kSelectedWeaponColor, true);
        // }

        SDL_Rect src_rect;
        if (weapon->GetObjectType() == ObjectType::MeleeWeapon) {
            src_rect = {weapon->GetTilePos().col * weapon->GetTilePos().w,
                        weapon->GetTilePos().row * weapon->GetTilePos().h,
                        weapon->GetTilePos().w, weapon->GetTilePos().h};

        } else {
            src_rect = {0, 0, weapon->GetWidth(), weapon->GetHeight()};
        }
        Box const weapon_box{weapon->GetTextureID(), src_rect, {0, 0, 0, 255}};
        box_container.AddBox(weapon_box);
    }
    box_container.DrawBoxes();
}

void WeaponInventory::Clean() {}

void WeaponInventory::Update(const Player& player) {
    m_Weapons = player.GetPlayerWeapons();
    m_SelectedWeapon = player.GetCurrentWeapon();
}
