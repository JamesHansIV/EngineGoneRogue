#include "Animation.h"

#include "Engine/Renderer/Renderer.h"
#include <utility>

void Animation::Update(){
    // need to work on how to get the animation to start from frame 0 when a new animation is played
    m_SpriteFrame = (SDL_GetTicks()/m_AnimSpeed) % m_FrameCount;
}

void Animation::Draw(const Rect& dstRect, float angle){
    SDL_Rect src_rect = { (m_TilePos.col + m_SpriteFrame) * m_TilePos.w, m_TilePos.row * m_TilePos.h, m_TilePos.w, m_TilePos.h };
    SDL_Rect dst_rect = { static_cast<int>(dstRect.x), static_cast<int>(dstRect.y), dstRect.w, dstRect.h  };

    Renderer::GetInstance()->Draw(m_TextureID, src_rect, dst_rect, angle, nullptr, m_Flip);
}

void Animation::SetProps(std::string textureID, TilePos tilePos, int frameCount, int animSpeed, SDL_RendererFlip flip){
    m_TextureID = std::move(textureID);
    m_TilePos = { tilePos.row, tilePos.col, tilePos.w, tilePos.h };

    m_FrameCount = frameCount;
    m_AnimSpeed = animSpeed;
    m_Flip = flip;
}
