#include "ExperienceBar.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/utils/utils.h"

const double kDstRectMultiplier = .5;

void ExperienceBar::Draw() const {
    Renderer renderer = Renderer::GetInstance();
    const int bar_height = 15;
    const int bar_border_width = 2;
    const int bar_max_width =
        Application::Get().GetWindowWidth() - bar_border_width * 2;
    const int bar_cur_width = m_xp * bar_max_width / m_max_xp;

    const Position pos{renderer.GetCameraX() + bar_border_width,
                       renderer.GetCameraY() +
                           Application::Get().GetWindowHeight() -
                           (bar_height + bar_border_width)};

    SDL_Rect experience_bar_container_rect =
        DrawBar(pos, Size{bar_max_width, bar_height});
    SDL_Rect experience_bar_rect =
        DrawBar(pos, Size{bar_cur_width, bar_height});
    SDL_Rect experience_bar_border =
        DrawBar({pos.x - bar_border_width, pos.y - bar_border_width},
                Size{bar_max_width + bar_border_width * 2,
                     bar_height + bar_border_width * 2});

    renderer.DrawRect(experience_bar_border, {155, 148, 150, 255}, true);
    renderer.DrawRect(experience_bar_container_rect, {39, 92, 171, 255}, true);

    renderer.DrawRect(experience_bar_rect, {44, 138, 220, 255}, true);

    const std::string level_display = "LV " + std::to_string(m_level);
    Texture* level_texture =
        renderer.AddTextTexture("level", level_display, {255, 255, 255, 255});
    const Position level_position{
        static_cast<int>(renderer.GetCameraX() +
                         Application::Get().GetWindowWidth() -
                         level_texture->GetWidth() * kDstRectMultiplier -
                         bar_border_width * 2),
        static_cast<int>(
            renderer.GetCameraY() + Application::Get().GetWindowHeight() -
            level_texture->GetHeight() * kDstRectMultiplier - bar_border_width),
    };

    SDL_Rect src_rect = {0, 0, level_texture->GetWidth(),
                         level_texture->GetHeight()};
    SDL_Rect dst_rect = {
        level_position.x, level_position.y,
        static_cast<int>(level_texture->GetWidth() * kDstRectMultiplier),
        static_cast<int>(level_texture->GetHeight() * kDstRectMultiplier)};

    renderer.Draw("level", src_rect, dst_rect, SDL_FLIP_NONE);
}

void ExperienceBar::Update(const Player& player) {
    m_xp = player.GetStats().GetExperience();
    m_level = player.GetStats().GetLevel();
}
