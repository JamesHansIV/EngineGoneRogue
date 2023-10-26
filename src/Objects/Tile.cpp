#include "Tile.h"
#include "../Graphics/TextureManager.h"


void Tile::Draw() {
    SDL_Rect srcRect = { m_TileColumn * m_SrcTileSize, m_TileRow * m_SrcTileSize, m_SrcTileSize, m_SrcTileSize };
    SDL_Rect dstRect = { (int)m_Transform->X, (int)m_Transform->Y, m_DestTileSize, m_DestTileSize };
    SDL_Log("srcRect: %d, %d", dstRect.x, dstRect.y);

    TextureManager::GetInstance()->Draw(m_TextureID, srcRect, dstRect);
}