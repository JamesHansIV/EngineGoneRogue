#include "Tile.h"
#include "Engine/Renderer/Renderer.h"


void Tile::Draw() {
    SDL_Rect src_rect = { m_TileColumn * m_SrcTileSize, m_TileRow * m_SrcTileSize, m_SrcTileSize, m_SrcTileSize };
    SDL_Rect dst_rect = { static_cast<int>(m_DstRect.x), static_cast<int>(m_DstRect.y), m_DstRect.w, m_DstRect.h };

    Renderer::GetInstance()->Draw(m_TextureID, src_rect, dst_rect);
}