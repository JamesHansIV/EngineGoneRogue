#include "Tile.h"
#include "Engine/Renderer/Renderer.h"


void Tile::Draw() {
    SDL_Rect srcRect = { m_TileColumn * m_SrcTileSize, m_TileRow * m_SrcTileSize, m_SrcTileSize, m_SrcTileSize };
    SDL_Rect dstRect = { (int)m_DstRect.x, (int)m_DstRect.y, m_DstRect.w, m_DstRect.h };

    Renderer::GetInstance()->Draw(m_TextureID, srcRect, dstRect);
}