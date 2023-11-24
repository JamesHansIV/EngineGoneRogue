#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "Engine/Renderer/TileMap.h"
#include "Engine/utils/utils.h"

struct AnimationInfo {
    std::string TextureID;
    TilePos Tile;
    int FrameCount;
    int AnimationSpeed;
    SDL_RendererFlip Flip = SDL_FLIP_NONE;
    bool Loop = false;
};

class Animation{
    public:
        Animation()
        : m_Info({"", {0, 0, 0, 0}, 0, 0, SDL_FLIP_NONE, false }),
        m_Stopped(false), m_SpriteFrame(0), m_CurrentAnimationID("") {}

        void Update();
        void Draw(const Rect& dstRect, float angle = 0.0f);
        void SetProps(AnimationInfo info) { m_Info = info; }
        std::string GetTextureID() { return m_Info.TextureID; }
        int GetCurrentFrame(){return m_SpriteFrame;}
        int GetSpriteRow() { return m_Info.Tile.row; }
        int GetSpriteCol() { return m_Info.Tile.col; }
        int GetSpriteWidth() { return m_Info.Tile.w; }
        int GetSpriteHeight() { return m_Info.Tile.h; }
        int GetFrameCount() { return m_Info.FrameCount; }
        int GetAnimationSpeed() { return m_Info.AnimationSpeed; }
        std::string GetAnimationID() { return m_CurrentAnimationID; }

        void StopAnimation();
        bool Stopped() { return m_Stopped; }
        void SelectAnimation(const std::string& id);
        void AddAnimation(std::string id, AnimationInfo info) { m_Animations[id] = info; m_CurrentAnimationID = id; }
    private:
        std::unordered_map<std::string, AnimationInfo> m_Animations;
        AnimationInfo m_Info;
        std::string m_CurrentAnimationID;
        int m_SpriteFrame;
        bool m_Stopped;
};
