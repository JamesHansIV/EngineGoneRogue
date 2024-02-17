#include "WeaponInventory.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "SDL2/SDL_render.h"

const DrawColor kSelectedWeaponColor{182, 69, 153, 150};

void WeaponInventory::Draw() {
    Renderer* renderer = Renderer::GetInstance();

    int x = 0;
    for (Weapon* weapon : m_Weapons) {
        if (m_SelectedWeapon == weapon) {
            SDL_Rect bg_rect = {x + renderer->GetCameraX(),
                                renderer->GetCameraY(), 50, 40};
            renderer->DrawRect(bg_rect, kSelectedWeaponColor, true);
        }

        SDL_Rect src_rect;
        SDL_Rect dst_rect;

        if (weapon->GetObjectType() == ObjectType::MeleeWeapon) {
            src_rect = {weapon->GetTilePos().col * weapon->GetTilePos().w,
                        weapon->GetTilePos().row * weapon->GetTilePos().h,
                        weapon->GetTilePos().w, weapon->GetTilePos().h};

            dst_rect = {x + renderer->GetCameraX() + 12,
                        renderer->GetCameraY() + 4, 25, 30};

        } else {
            src_rect = {0, 0, weapon->GetWidth(), weapon->GetHeight()};
            dst_rect = {x + renderer->GetCameraX(), renderer->GetCameraY(), 50,
                        32};
        }

        renderer->Draw(weapon->GetTextureID(), src_rect, dst_rect, 0.0F,
                       nullptr, SDL_FLIP_NONE);
        SDL_Rect box = {x + renderer->GetCameraX(), renderer->GetCameraY(), 50,
                        40};
        renderer->DrawRect(box, {0, 0, 0, 255});

        x += 50;
    }
    SDL_Rect box = {renderer->GetCameraX(), renderer->GetCameraY(), x, 40};
    renderer->DrawRect(box, {0, 0, 0, 255});
}

void WeaponInventory::Clean() {}

void WeaponInventory::Update(const Player& player) {
    m_Weapons = player.GetPlayerWeapons();
    m_SelectedWeapon = player.GetCurrentWeapon();
}
