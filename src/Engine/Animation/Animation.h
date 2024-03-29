#pragma once
#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
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

    Animation(Animation* rhs);

    ~Animation() = default;

    void Update();
    void Draw(const Rect& dstRect, float angle = 0.0F);

    void SetProps(AnimationInfo info) { m_info = std::move(info); }

    [[nodiscard]] std::string GetTextureID() const { return m_info.TextureID; }

    [[nodiscard]] int GetCurrentFrame() const { return m_sprite_frame; }

    [[nodiscard]] int GetSpriteRow() const { return m_info.Tile.row; }

    [[nodiscard]] int GetSpriteCol() const { return m_info.Tile.col; }

    [[nodiscard]] int GetSpriteWidth() const { return m_info.Tile.w; }

    [[nodiscard]] int GetSpriteHeight() const { return m_info.Tile.h; }

    [[nodiscard]] int GetFrameCount() const { return m_info.FrameCount; }

    [[nodiscard]] int GetAnimationSpeed() const {
        return m_info.AnimationSpeed;
    }

    void SetAnimationSpeed(int speed) { m_info.AnimationSpeed = speed; }

    std::string GetAnimationID() { return m_current_animation_id; }

    std::map<std::string, AnimationInfo> GetAnimations() {
        return m_animations;
    }

    std::map<std::string, AnimationInfo> GetAnimationInfo() {
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

    [[nodiscard]] bool OnKeyFrame() const {
        return m_info.KeyFramesStart <= m_sprite_frame &&
               m_sprite_frame < m_info.KeyFramesEnd;
    }

    [[nodiscard]] bool Ended() const { return m_ended; }

    void StopAnimation();

    [[nodiscard]] bool LastFrame() const {
        return m_sprite_frame + 1 == m_info.FrameCount;
    }

    void SelectAnimation(const std::string& id);

    void AddAnimation(const std::string& id, AnimationInfo info) {
        m_animations[id] = std::move(info);
        m_current_animation_id = id;
    }

   private:
    std::map<std::string, AnimationInfo> m_animations;
    AnimationInfo m_info;
    std::string m_current_animation_id;
    int m_sprite_frame{};
    bool m_ended{};
    double m_last_update_time{};
};
