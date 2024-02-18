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
    Animation()
        : m_Info({"", {0, 0, 0, 0}, 0, 0, SDL_FLIP_NONE, false}),
          m_Ended(false),
          m_SpriteFrame(0),
          m_CurrentAnimationID(""),
          m_LastFrameTime(0) {}

    void Update();
    void Draw(const Rect& dstRect, float angle = 0.0f);

    void SetProps(AnimationInfo info) { m_Info = info; }

    std::string GetTextureID() const { return m_Info.TextureID; }

    int GetCurrentFrame() const { return m_SpriteFrame; }

    int GetSpriteRow() const { return m_Info.Tile.row; }

    int GetSpriteCol() const { return m_Info.Tile.col; }

    int GetSpriteWidth() const { return m_Info.Tile.w; }

    int GetSpriteHeight() const { return m_Info.Tile.h; }

    int GetFrameCount() const { return m_Info.FrameCount; }

    int GetAnimationSpeed() const { return m_Info.AnimationSpeed; }

    std::string GetAnimationID() { return m_CurrentAnimationID; }

    std::unordered_map<std::string, AnimationInfo> GetAnimations() {
        return m_Animations;
    }

    std::unordered_map<std::string, AnimationInfo> GetAnimationInfo() {
        return m_Animations;
    }

    void GetKeyFrameRange(int& start, int& end) const {
        start = m_Info.KeyFramesStart;
        end = m_Info.KeyFramesEnd;
    }

    void SetKeyFrameRange(int start, int end) {
        m_Info.KeyFramesStart = start;
        m_Info.KeyFramesEnd = end;
    }

    bool OnKeyFrame() const {
        return m_Info.KeyFramesStart <= m_SpriteFrame &&
               m_SpriteFrame < m_Info.KeyFramesEnd;
    }

    bool Ended() const { return m_Ended; }

    void StopAnimation();

    bool LastFrame() { return m_SpriteFrame + 1 == m_Info.FrameCount; }

    void SelectAnimation(const std::string& id);

    void AddAnimation(std::string id, AnimationInfo info) {
        m_Animations[id] = info;
        m_CurrentAnimationID = id;

        SDL_Log("added animation %s in texture id: %s",
                m_CurrentAnimationID.c_str(),
                m_Animations[id].TextureID.c_str());
    }

   private:
    std::unordered_map<std::string, AnimationInfo> m_Animations;
    AnimationInfo m_Info;
    std::string m_CurrentAnimationID;
    int m_SpriteFrame;
    bool m_Ended;
    double m_LastFrameTime;
};
