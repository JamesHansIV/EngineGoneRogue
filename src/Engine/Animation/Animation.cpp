#include "Animation.h"

#include <utility>
#include "Engine/Application/Application.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"
#include "SDL2/SDL_render.h"

Animation::Animation(Animation* rhs) {
    m_current_animation_id = rhs->m_current_animation_id;
    m_info = rhs->m_info;

    m_animations = rhs->m_animations;

    m_ended = false;
    m_last_update_time = 0;
    m_sprite_frame = 0;
}

void Animation::Update() {

    bool const no_update = LastFrame() && !m_info.Loop;

    if (no_update) {
        m_ended = true;
        return;
    }

    if (!m_ended &&
        (timer.GetTicks() - m_last_update_time) > m_info.AnimationSpeed) {
        //SDL_Log("current frame: %d", m_SpriteFrame);

        m_sprite_frame = (m_sprite_frame + 1) % m_info.FrameCount;
        // SDL_Log("Updating animation frame: %d", m_SpriteFrame);
        m_last_update_time = timer.GetTicks();
    }
}

void Animation::Draw(const Rect& dstRect, float angle) {
    SDL_Rect src_rect = {(m_info.Tile.col + m_sprite_frame) * m_info.Tile.w,
                         m_info.Tile.row * m_info.Tile.h, m_info.Tile.w,
                         m_info.Tile.h};
    SDL_Rect dst_rect = {static_cast<int>(dstRect.x),
                         static_cast<int>(dstRect.y), dstRect.w, dstRect.h};

    Renderer::GetInstance().Draw(m_info.TextureID, src_rect, dst_rect, angle,
                                 nullptr, m_info.Flip);
}

void Animation::StopAnimation() {
    m_sprite_frame = m_info.FrameCount - 1;
}

void Animation::SelectAnimation(const std::string& id) {
    if (m_animations.find(id) == m_animations.end()) {
        SDL_LogError(0, "Animation %s does not exist", id.c_str());
    }
    m_sprite_frame = 0;
    m_ended = false;
    SetProps(m_animations[id]);
    m_current_animation_id = id;
}
