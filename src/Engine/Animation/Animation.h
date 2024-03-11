#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include "Engine/Renderer/TileMap.h"
#include "Engine/utils/utils.h"

struct AnimationInfo {
    std::string TextureID;
    TilePos Tile;
    int FrameCount;
    int AnimationSpeed;
    SDL_RendererFlip Flip = SDL_FLIP_NONE;
    bool Loop = false;
    int KeyFramesStart = 0;
    int KeyFramesEnd = 0;
};

class Animation {
   public:
    Animation() : m_info({"", {0, 0, 0, 0}, 0, 0, SDL_FLIP_NONE, false}) {}

    Animation(Animation& rhs);

    void Update();
    void Draw(const Rect& dstRect, float angle = 0.0F);

    void SetProps(AnimationInfo info) { m_info = info; }

    std::string GetTextureID() const { return m_info.TextureID; }

    int GetCurrentFrame() const { return m_sprite_frame; }

    int GetSpriteRow() const { return m_info.Tile.row; }

    int GetSpriteCol() const { return m_info.Tile.col; }

    int GetSpriteWidth() const { return m_info.Tile.w; }

    int GetSpriteHeight() const { return m_info.Tile.h; }

    int GetFrameCount() const { return m_info.FrameCount; }

    int GetAnimationSpeed() const { return m_info.AnimationSpeed; }

    std::string GetAnimationID() { return m_current_animation_id; }

    std::unordered_map<std::string, AnimationInfo> GetAnimations() {
        return m_animations;
    }

    std::unordered_map<std::string, AnimationInfo> GetAnimationInfo() {
        return m_animations;
    }

    void GetKeyFrameRange(int& start, int& end) const {
        start = m_info.KeyFramesStart;
        end = m_info.KeyFramesEnd;
    }

    void SetKeyFrameRange(int start, int end) {
        m_info.KeyFramesStart = start;
        m_info.KeyFramesEnd = end;
    }

    bool OnKeyFrame() const {
        return m_info.KeyFramesStart <= m_sprite_frame &&
               m_sprite_frame < m_info.KeyFramesEnd;
    }

    bool Ended() const { return m_ended; }

    void StopAnimation();

    bool LastFrame() const { return m_sprite_frame + 1 == m_info.FrameCount; }

    void SelectAnimation(const std::string& id);

    void AddAnimation(std::string id, AnimationInfo info) {
        m_animations[id] = info;
        m_current_animation_id = id;

        SDL_Log("added animation %s in texture id: %s",
                m_CurrentAnimationID.c_str(),
                m_animations[id].TextureID.c_str());
    }

   private:
    std::unordered_map<std::string, AnimationInfo> m_animations;
    AnimationInfo m_info;
    std::string m_current_animation_id;
    int m_sprite_frame{};
    bool m_ended{};
    double m_last_update_time{};
};
