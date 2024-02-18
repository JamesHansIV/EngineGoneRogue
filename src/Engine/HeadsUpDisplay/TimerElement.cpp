#include "TimerElement.h"
#include "Engine/Application/Application.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"

Renderer* renderer = Renderer::GetInstance();
const int kDstRectMultiplier = 2;
const SDL_Color kTimerColor = {255, 255, 255, 255};

TimerElement::TimerElement() = default;

std::string GetFormattedTime() {
    const Uint32 raw_seconds = (timer.GetTicks() / 1000);
    const Uint32 seconds = raw_seconds % 60;
    const Uint32 minutes = raw_seconds / 60;
    std::string time;

    if (seconds < 10) {
        time = std::to_string(minutes) + ":0" + std::to_string(seconds % 60);
    } else {
        time = std::to_string(minutes) + ":" + std::to_string(seconds % 60);
    }

    return time;
}

void TimerElement::Draw() {
    const std::string time = GetFormattedTime();

    Texture* timer_texture =
        renderer->AddTextTexture("timer", time, kTimerColor);
    // Timer should be drawn at the top middle of the screen
    const Position timer_position{renderer->GetCameraX() +
                                      Application::Get()->GetWindowWidth() / 2 -
                                      timer_texture->GetWidth(),
                                  renderer->GetCameraY()};

    SDL_Rect src_rect = {0, 0, timer_texture->GetWidth(),
                         timer_texture->GetHeight()};
    SDL_Rect dst_rect = {timer_position.x, timer_position.y,
                         timer_texture->GetWidth() * kDstRectMultiplier,
                         timer_texture->GetHeight() * kDstRectMultiplier};

    renderer->Draw("timer", src_rect, dst_rect, SDL_FLIP_NONE);
}
