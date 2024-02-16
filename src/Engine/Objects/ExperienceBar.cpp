#include "ExperienceBar.h"
#include <SDL2/SDL.h>
#include "Engine/Objects/utils/utils.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/utils/utils.h"

void ExperienceBar::Draw() const {

    const Position pos{
        Renderer::GetInstance()->GetCameraX(),
        Renderer::GetInstance()->GetCameraY() + SCREEN_HEIGHT - 10};

    SDL_Rect experience_bar_container_rect = DrawBar(pos, Size{m_max_xp, 10});
    SDL_Rect experience_bar_rect = DrawBar(pos, Size{m_xp, 5});

    Renderer::GetInstance()->DrawRect(experience_bar_container_rect,
                                      {39, 92, 171, 255}, true);

    Renderer::GetInstance()->DrawRect(experience_bar_rect, {44, 138, 220, 255},
                                      true);
}
