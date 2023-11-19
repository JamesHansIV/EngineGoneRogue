#include "Health.h"
#include "Engine/utils/utils.h"
#include "Engine/Renderer/Renderer.h"
#include <SDL2/SDL.h>

void Health::Draw(int objX, int objY, int objWidth) const {
    
    int health_bar_width = static_cast<int>((objWidth + 15) * (m_Health / static_cast<float>(m_TotalHealth)));
    
    if (health_bar_width <= 0) health_bar_width = 0;
    int const health_bar_x = objX - m_XOffset;
    int const health_bar_y = objY - HEALTH_BAR_HEIGHT - m_YOffset;

    SDL_Rect health_bar_rect;
    health_bar_rect.x = health_bar_x;
    health_bar_rect.y = health_bar_y;
    health_bar_rect.w = health_bar_width;
    health_bar_rect.h = HEALTH_BAR_HEIGHT;

    Renderer::GetInstance()->DrawRect(health_bar_rect, {0, 255, 0, 255}, true);
}