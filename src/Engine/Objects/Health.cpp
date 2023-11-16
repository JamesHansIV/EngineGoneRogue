#include "Health.h"
#include "Engine/utils/utils.h"
#include "Engine/Renderer/Renderer.h"
#include <SDL2/SDL.h>

void Health::Draw(int objX, int objY, int objWidth, int objHeight) {
    
    int healthBarWidth = static_cast<int>((objWidth + 15) * (m_Health / (float)m_TotalHealth));
    
    if (healthBarWidth <= 0) healthBarWidth = 0;
    int healthBarX = objX - m_XOffset;
    int healthBarY = objY - HEALTH_BAR_HEIGHT - m_YOffset;

    SDL_Rect healthBarRect;
    healthBarRect.x = healthBarX;
    healthBarRect.y = healthBarY;
    healthBarRect.w = healthBarWidth;
    healthBarRect.h = HEALTH_BAR_HEIGHT;

    Renderer::GetInstance()->DrawRect(healthBarRect, {0, 255, 0, 255}, true);
}