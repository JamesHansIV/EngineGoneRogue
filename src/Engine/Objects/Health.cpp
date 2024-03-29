#include "Health.h"
#include <SDL2/SDL.h>
#include "Engine/Objects/utils/utils.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/utils/utils.h"

void Health::Draw(int objX, int objY, int objWidth) const {

    const int health_bar_container_width = objWidth + 15;
    int health_bar_width =
        static_cast<int>(health_bar_container_width *
                         (m_hp / static_cast<float>(m_total_health)));

    if (health_bar_width <= 0) {
        health_bar_width = 0;
    }

    const Position pos{objX - m_x_offset, objY - m_y_offset};
    SDL_Rect health_bar_rect =
        DrawBar(pos, Size{health_bar_width, kHealthBarHeight});
    SDL_Rect health_bar_container_rect =
        DrawBar(pos, Size{health_bar_container_width, kHealthBarHeight});

    Renderer::GetInstance().DrawRect(health_bar_container_rect,
                                     {53, 18, 104, 255}, true);
    Renderer::GetInstance().DrawRect(health_bar_rect, {182, 69, 153, 255},
                                     true);
}

void Health::IncreaseHealth(int amount) {
    m_hp += amount;
    if (m_hp > m_total_health) {
        m_hp = m_total_health;
    }
}
