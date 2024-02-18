#include "Engine/Objects/ItemInventory.h"
#include "Engine/Objects/BoxContainer.h"
#include "Engine/Objects/Characters/Player.h"

const DrawColor kWeaponColor{0, 0, 0, 255};

void ItemInventory::Draw() {
    BoxContainer box_container(0, 40, 40, 50, 0);

    for (auto& pair : m_Items) {
        const Item& item = *pair.second;
        
        SDL_Rect src_rect = {0, 0, 16, 16};
        Box const item_box{item.GetTexture(), src_rect, kWeaponColor, false, 
            true, item.GetCount()};
        box_container.AddBox(item_box);
    }
    
    box_container.DrawBoxes();
}

void ItemInventory::Update(const Player& player){
    m_Items = player.GetPlayerItems();
}