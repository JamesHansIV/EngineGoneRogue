#pragma once
#include <unordered_map>
#include <utility>
#include <vector>
#include "Engine/Objects/Item.h"
#include "Engine/UI/Button.h"

const int kCellHeight = 40;
const int kCellWidth = 50;

class ChestDropModal {
   public:
    explicit ChestDropModal() = default;

    explicit ChestDropModal(std::vector<ItemType> items)
        : m_items(std::move(items)) {
        int const window_width = Application::Get().GetWindowWidth();
        int const window_height = Application::Get().GetWindowHeight();

        int const grid_height = static_cast<int>(m_items.size()) * kCellHeight;

        int const button_width = 100;
        int const button_height = 60;
        int const button_x = (window_width - button_width) / 2;
        int const button_y = (window_height + grid_height) / 2;

        m_button =
            Button("buttons",
                   SDL_Rect{button_x, button_y, button_width, button_height},
                   "Continue", []() {
                       SDL_Log("Continue button clicked");
                       timer.Unpause();
                       PushNewEvent(EventType::ContinueGameEvent);
                   });
    };

    void Draw();

    void Clean();

    void Update();

   private:
    std::vector<ItemType> m_items;
    Button m_button;
};
