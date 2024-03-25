#pragma once

#include "Engine/Renderer/TileMap.h"
#include "Engine/utils/utils.h"

class DrawElement {
   public:
    DrawElement() = default;

    DrawElement(std::string textureID, SDL_Rect dst)
        : m_TextureID(textureID), m_Dst(dst) {}

    void Draw(TilePos src);

    void ChangeDst(SDL_Rect dst) { m_Dst = dst; }

   private:
    SDL_Rect m_Dst;
    std::string m_TextureID;
};
