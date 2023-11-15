#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "Engine/Renderer/TileMap.h"
#include "Engine/utils/utils.h"

class Animation{
    public:
        Animation()= default;

        void Update();
        void Draw(const Rect& dstRect, float angle = 0.0f);
        void SetProps(std::string textureID, TilePos tilePos, int frameCount, int animSpeed, SDL_RendererFlip flip=SDL_FLIP_NONE);
        std::string GetTextureID() { return m_TextureID; }
        int GetCurrentFrame(){return m_SpriteFrame;}
        int GetSpriteRow() { return m_TilePos.row; }
        int GetSpriteCol() { return m_TilePos.col; }
        int GetSpriteWidth() { return m_TilePos.w; }
        int GetSpriteHeight() { return m_TilePos.h; }
        int GetFrameCount() { return m_FrameCount; }
        int GetAnimationSpeed() { return m_AnimSpeed; }
    private:
        int m_SpriteRow, m_SpriteCol, m_SpriteTileSize, m_SpriteFrame;
        int m_AnimSpeed, m_FrameCount;
        TilePos m_TilePos {0, 0, 0, 0};
        std::string m_TextureID;
        SDL_RendererFlip m_Flip;
};
