#include "ItemInventory.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/UI/GridComponent.h"

const DrawColor kColor{0, 0, 0, 255};
const int kCellHeight = 60;
const int kCellWidth = 50;

void ItemInventory::Draw() {
    GridComponent box_container(1, static_cast<int>(m_items.size()), 0, 40, kCellHeight, kCellWidth);

    int column = 0;
    for (const auto& pair : m_items) {
        const Item& item = *pair.second;
        SDL_Rect const src_rect = {0, 0, 19, 38};
        auto* item_box = new BoxWithCounter( item.GetCount(),item.GetTexture(), src_rect, kColor, false);
        box_container.AddBox(item_box, 0, column);
        column++;
    }

    box_container.DrawGrid();
}

void ItemInventory::Update(const Player& player) {
    m_items = player.GetPlayerItems();
}