#include "Animation.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Application/Application.h"
#include <utility>

void Animation::Update(){
    // need to work on how to get the animation to start from frame 0 when a new animation is played
    // SDL_Log("global frame: %d", Application::Get()->GetFrame());


    if (!m_Stopped && m_SpriteFrame + 1 == m_Info.FrameCount && !m_Info.Loop) {
        StopAnimation();
        return;
    }

    if (!m_Stopped && Application::Get()->GetFrame() % m_Info.AnimationSpeed == 0) {
        // SDL_Log("current frame: %d", m_SpriteFrame);
        
        m_SpriteFrame = (m_SpriteFrame + 1) % m_Info.FrameCount;
        // SDL_Log("Updating animation frame: %d", m_SpriteFrame);
    }
}

void Animation::Draw(const Rect& dstRect, float angle){
    SDL_Rect src_rect = { (m_Info.Tile.col + m_SpriteFrame) * m_Info.Tile.w, m_Info.Tile.row * m_Info.Tile.h, m_Info.Tile.w, m_Info.Tile.h };
    SDL_Rect dst_rect = { static_cast<int>(dstRect.x), static_cast<int>(dstRect.y), dstRect.w, dstRect.h  };

    Renderer::GetInstance()->Draw(m_Info.TextureID, src_rect, dst_rect, angle, nullptr, m_Info.Flip);
}

void Animation::StopAnimation() {
    m_Stopped = true;
}

void Animation::SelectAnimation(std::string id) {
    if (m_Animations.find(id) == m_Animations.end()) {
        SDL_LogError(0, "Animation does not exist");
    }
    m_SpriteFrame = 0;
    m_Stopped = false;
    SetProps(m_Animations[id]);
}