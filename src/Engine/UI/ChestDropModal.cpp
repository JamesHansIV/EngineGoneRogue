#include "ChestDropModal.h"
#include "Engine/Events/Event.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/UI/GridComponent.h"
#include "Apps/Game.h"

const DrawColor kColor{0, 0, 0, 255};
const DrawColor kTransparentColor { 0, 0, 0, 0};
const DrawColor kBorderColor = {255,255,255,255};

const int kColumn1Width = 190;
const int kColumn2Width = 550;

const int kChestTextureHeight = 150;
const int kChestTextureWidth = 250;

void ChestDropModal::Draw()
{
    int const window_width = Application::Get()->GetWindowWidth();
    int const window_height = Application::Get()->GetWindowHeight();

    int const grid_height = static_cast<int>(m_items.size()) * kCellHeight;
    int const grid_width = kCellWidth + kColumn1Width + kColumn2Width;

    int const x = (window_width - grid_width) / 4;
    int const y = (window_height - grid_height) / 2 ;

    GridComponent grid_container(1, 1, x, y - kCellHeight, kCellHeight, grid_width);
    Box* message_box = new BoxWithText("Chest Found! You found these items:", {0, 0, 0, 255}, kColor, true, TextAlign::KCenter);
    grid_container.AddBox(message_box, 0, 0);
    grid_container.DrawGrid();
    
    GridComponent box_container(static_cast<int>(m_items.size()),3, x, y, kCellHeight, kCellWidth);
    box_container.SetColumnWidth(1, kColumn1Width);
    box_container.SetColumnWidth(2, kColumn2Width);
    int row = 0;
    for (ItemType const item_t: m_items) {
        Properties props("", {0, 0, 16, 16}, {0,0, 32, 32}, 0, "");
        Item item(props, item_t);
        SDL_Rect const src_rect = {0, 0, 19, 38};
        Box* item_box = new BoxWithTexture(item.GetTextureID(), src_rect,kColor,false);
        Box* item_name = new BoxWithText(item.GetName(), src_rect, kColor, true, TextAlign::KCenter);
        Box* item_description = new BoxWithText(item.GetDescription(), src_rect, kColor, true, TextAlign::KLeft);
        box_container.AddBox(item_box, row, 0);
        box_container.AddBox(item_name, row, 1);
        box_container.AddBox(item_description, row, 2);
        row++;
    }

    box_container.SetGridBorderColor(kBorderColor);
    box_container.DrawGrid();
    
    int const chest_x = (window_width - kChestTextureWidth)/4;
    int const chest_y = (window_height - grid_height)/2 - kCellHeight - kChestTextureHeight;
    GridComponent chest_grid(1, 1, chest_x, chest_y, kChestTextureHeight, kChestTextureWidth); // Adjust the size to 100x100
    Box* chest_box = new BoxWithTexture("wooden-chest-idle", {1, 5, 18, 16}, kTransparentColor, false);
    chest_grid.AddBox(chest_box, 0, 0);
    chest_grid.DrawGrid();
    
    m_button.Draw();
}

void ChestDropModal::Update()
{
   m_button.Update();
}