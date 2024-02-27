#include "Animation.h"

#include <utility>
#include "Engine/Application/Application.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"
#include "SDL2/SDL_render.h"

Animation::Animation(Animation& rhs) {
    m_CurrentAnimationID = rhs.m_CurrentAnimationID;
    m_Info = rhs.m_Info;

    for (auto item : rhs.m_Animations) {
        SDL_Log("copying %s", item.first.c_str());
        SDL_Log("info: %s, %d, %d", item.second.TextureID.c_str(),
                item.second.FrameCount, item.second.AnimationSpeed);

        AnimationInfo info = {
            item.second.TextureID,      item.second.Tile,
            item.second.FrameCount,     item.second.AnimationSpeed,
            item.second.Flip,           item.second.Loop,
            item.second.KeyFramesStart, item.second.KeyFramesEnd};

        m_Animations[item.first] = info;
    }
    m_Ended = false;
    m_LastUpdateTime = 0;
    m_SpriteFrame = 0;
}

void Animation::Update() {

    bool const no_update = LastFrame() && !m_Info.Loop;

    if (no_update) {
        m_Ended = true;
        return;
    }

    if (!m_Ended &&
        (timer.GetTicks() - m_LastUpdateTime) > m_Info.AnimationSpeed) {
        //SDL_Log("current frame: %d", m_SpriteFrame);

        m_SpriteFrame = (m_SpriteFrame + 1) % m_Info.FrameCount;
        // SDL_Log("Updating animation frame: %d", m_SpriteFrame);
        m_LastUpdateTime = timer.GetTicks();
    }
}

void Animation::Draw(const Rect& dstRect, float angle) {
    SDL_Rect src_rect = {(m_Info.Tile.col + m_SpriteFrame) * m_Info.Tile.w,
                         m_Info.Tile.row * m_Info.Tile.h, m_Info.Tile.w,
                         m_Info.Tile.h};
    SDL_Rect dst_rect = {static_cast<int>(dstRect.x),
                         static_cast<int>(dstRect.y), dstRect.w, dstRect.h};

    Renderer::GetInstance()->Draw(m_Info.TextureID, src_rect, dst_rect, angle,
                                  nullptr, m_Info.Flip);
}

void Animation::StopAnimation() {
    m_SpriteFrame = m_Info.FrameCount - 1;
}

void Animation::SelectAnimation(const std::string& id) {
    if (m_Animations.find(id) == m_Animations.end()) {
        SDL_LogError(0, "Animation %s does not exist", id.c_str());
    }
    m_SpriteFrame = 0;
    m_Ended = false;
    SetProps(m_Animations[id]);
    m_CurrentAnimationID = id;
}
