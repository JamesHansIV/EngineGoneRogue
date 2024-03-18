#include "Engine/Objects/ChestDropModal.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/GridComponent.h"
#include "Apps/Game.h"

const DrawColor kColor{0, 0, 0, 255};

void ChestDropModal::Draw()
{
    int const window_width = Application::Get()->GetWindowWidth();
    int const window_height = Application::Get()->GetWindowHeight();

    int const grid_height = static_cast<int>(m_items.size()) * kCellHeight;
    int const grid_width = kCellWidth + 550;

    int const x = (window_width - grid_width) / 4;
    int const y = (window_height - grid_height) / 2 ;

    GridComponent grid_container(1, 1, x, y - kCellHeight, kCellHeight, grid_width);
    Box* message_box = new BoxWithText("Chest Found! You found these items:", {0, 0, 0, 255}, kColor, true, TextAlign::KCenter);
    grid_container.AddBox(message_box, 0, 0);
    grid_container.DrawGrid();
    
    GridComponent box_container(static_cast<int>(m_items.size()),2, x, y, kCellHeight, kCellWidth);
    box_container.SetColumnWidth(1, 550);
    int row = 0;
    for (ItemType const item_t: m_items) {
        Properties props("", {0, 0, 16, 16}, {0,0, 32, 32}, 0, "");
        Item item(props, item_t);
        SDL_Rect const src_rect = {0, 0, 16, 16};
        Box* item_box = new BoxWithTexture(item.GetTextureID(), src_rect,kColor,true);
        Box* item_description = new BoxWithText(item.GetDescription(), src_rect, kColor, true, TextAlign::KLeft);
        box_container.AddBox(item_box, row, 0);
        box_container.AddBox(item_description, row, 1);
        row++;
    }
    
    box_container.DrawGrid();
    
    int const chest_x = (window_width - 150)/4;
    int const chest_y = (window_height - grid_height)/2 - kCellHeight - 150;
    GridComponent chest_grid(1, 1, chest_x, chest_y, 150, 150); // Adjust the size to 100x100
    Box* chest_box = new BoxWithTexture("wooden-chest-idle", {0, 0, 16, 16}, kColor, false);
    chest_grid.AddBox(chest_box, 0, 0);
    chest_grid.DrawGrid();
    
    m_button.Draw();
}

void ChestDropModal::Update()
{
   m_button.Update();
}