#include "Engine/Objects/InventoryComponent.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/UI/GridComponent.h"

const DrawColor kColor{0, 0, 0, 255};
const int kCellHeight = 30;
const int kCellWidth = 50;
const DrawColor kBorderColor = {255,255,255,255};

void InventoryComponent::Draw() {
    GridComponent box_container(static_cast<int>(m_items.size()) + 1, 3, 0, 530, kCellHeight, kCellWidth);

    int row = 0;

    SDL_Rect const src_rect = {0, 0, 19, 38};

    auto* header_box_texture = new BoxWithText(".", src_rect, kColor, true, KCenter);
    box_container.AddBox(header_box_texture, row, 0);

    auto* header_box_name = new BoxWithText("Name", src_rect, kColor, true, KCenter);
    box_container.AddBox(header_box_name, row, 1);

    auto* header_box_count = new BoxWithText("Count", src_rect, kColor, true, KCenter);
    box_container.AddBox(header_box_count, row, 2);

    row++;

    for (const auto& pair : m_items) {
        const Item& item = *pair.second;

        auto* item_box_texture = new BoxWithTexture(item.GetTexture(), src_rect, kColor, false);
        box_container.AddBox(item_box_texture, row, 0);

        auto* item_box_name = new BoxWithText(item.GetName(), src_rect, kColor, false, KCenter);
        box_container.AddBox(item_box_name, row, 1);

        auto* item_box_count = new BoxWithText(std::to_string(item.GetCount()), src_rect, kColor, false, KCenter);
        box_container.AddBox(item_box_count, row, 2);

        row++;
    }
    
    box_container.SetColumnWidth(1, 200);
    box_container.SetColumnWidth(2, 100);
    box_container.SetGridBorderColor(kBorderColor);
    box_container.DrawGrid();


    GridComponent box_container2(1, 1, 0, 500, kCellHeight, kCellWidth);
    auto* header = new BoxWithText("Item Inventory", src_rect, kColor, false, KCenter);
    box_container2.AddBox(header, 0, 0);
    int total_width = box_container.GetColumnWidth(0) + box_container.GetColumnWidth(1) + box_container.GetColumnWidth(2);
    box_container2.SetColumnWidth(0, total_width);
    box_container2.SetGridBorderColor(kBorderColor);
    box_container2.DrawGrid();

}

void InventoryComponent::Update(const Player& player) {
    m_items = player.GetPlayerItems();
}