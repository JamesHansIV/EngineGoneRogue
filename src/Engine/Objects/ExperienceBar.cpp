#include "ExperienceBar.h"
#include <SDL2/SDL.h>
#include "Engine/Objects/utils/utils.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/utils/utils.h"

void ExperienceBar::Draw() const {
    Renderer* renderer = Renderer::GetInstance();

    const Position pos{renderer->GetCameraX(),
                       renderer->GetCameraY() + SCREEN_HEIGHT - 10};

    SDL_Rect experience_bar_container_rect = DrawBar(pos, Size{m_max_xp, 10});
    SDL_Rect experience_bar_rect = DrawBar(pos, Size{m_xp, 10});

    renderer->DrawRect(experience_bar_container_rect, {39, 92, 171, 255}, true);

    renderer->DrawRect(experience_bar_rect, {44, 138, 220, 255}, true);
}
