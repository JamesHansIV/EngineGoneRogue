#include "WeaponInventory.h"
#include "Engine/Objects/IObject.h"
#include "SDL2/SDL_render.h"
#include "Engine/Objects/Weapons/Weapon.h"

void WeaponInventory::Draw() {
   SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();
   int x = 0;
   for(Weapon* weapon : m_Weapons) {
      if(m_SelectedWeapon == weapon){
         SDL_Rect bg_rect = {x+Renderer::GetInstance()->GetCameraX(), Renderer::GetInstance()->GetCameraY(), 50, 40};
         Renderer::GetInstance()->DrawRect(bg_rect, {255,255,255,255}, true);
      }

      SDL_Rect src_rect;
      SDL_Rect dst_rect;

      if(weapon->GetObjectType() == ObjectType::MeleeWeapon){
         src_rect = {weapon->GetTilePos().col * weapon->GetTilePos().w,
                         weapon->GetTilePos().row * weapon->GetTilePos().h,
                         weapon->GetTilePos().w, weapon->GetTilePos().h};

         dst_rect = {x+Renderer::GetInstance()->GetCameraX()+12, Renderer::GetInstance()->GetCameraY()+4, 25, 30};

      } else {
         src_rect = {0, 0, weapon->GetWidth(), weapon->GetHeight()}; 
         dst_rect = {x+Renderer::GetInstance()->GetCameraX(), Renderer::GetInstance()->GetCameraY(), 50, 32};
      }

      
      Renderer::GetInstance()->Draw(weapon->GetTextureID(), src_rect, dst_rect, 0.0F, nullptr, SDL_FLIP_NONE);
      SDL_Rect box = {x+Renderer::GetInstance()->GetCameraX(),Renderer::GetInstance()->GetCameraY(),50, 40};
      Renderer::GetInstance()->DrawRect(box, {0, 0, 0, 255});
      
      x += 50;
   }
   SDL_Rect box = {Renderer::GetInstance()->GetCameraX(),Renderer::GetInstance()->GetCameraY(),x, 40};
   Renderer::GetInstance()->DrawRect(box, {0, 0, 0, 255});
}

void WeaponInventory::Clean() {

}

void WeaponInventory::Update(float dt) {

}