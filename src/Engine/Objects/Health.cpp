#include "Health.h"
#include <SDL2/SDL.h>
#include "Engine/Objects/utils/utils.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/utils/utils.h"

void Health::Draw(int objX, int objY, int objWidth) const {

    const int health_bar_container_width = objWidth + 15;
    int health_bar_width = static_cast<int>(
        (objWidth + 15) * (m_HP / static_cast<float>(m_TotalHealth)));

    if (health_bar_width <= 0) {
        health_bar_width = 0;
    }

    const Position pos{objX - m_XOffset, objY - HEALTH_BAR_HEIGHT - m_YOffset};
    SDL_Rect health_bar_rect =
        DrawBar(pos, Size{health_bar_width, HEALTH_BAR_HEIGHT});
    SDL_Rect health_bar_container_rect =
        DrawBar(pos, Size{health_bar_container_width, HEALTH_BAR_HEIGHT});

    Renderer::GetInstance()->DrawRect(health_bar_container_rect,
                                      {255, 0, 0, 255}, true);
    Renderer::GetInstance()->DrawRect(health_bar_rect, {0, 255, 0, 255}, true);
}

void Health::IncreaseHealth(int amount) {
    m_HP += amount;
    if (m_HP > m_TotalHealth) {
        m_HP = m_TotalHealth;
    }
}
