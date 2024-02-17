#include "Engine/HeadsUpDisplay/HeadsUpDisplay.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"

HUD::HUD() = default;

void HUD::Draw() const {
    Renderer* renderer = Renderer::GetInstance();
    // Timer
    m_TimerElement.Draw();
}
