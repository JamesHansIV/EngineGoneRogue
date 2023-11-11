#include "TileMap.h"

#include <utility>

TileMap::TileMap(const std::string& path, std::string textureID, int tileSize, int rows, int cols)
    : Texture(path, std::move(textureID)), m_TileSize(tileSize), m_Rows(rows), m_Cols(cols) {

        for(int i = 0; i < m_Rows; i++) {
            m_ActiveTileButtons.emplace_back();
            for (int j = 0; j < m_Cols; j++) {
                m_ActiveTileButtons[i].push_back(false);
            }
        }
    }
    
TileMap::TileMap(const char* path, std::string id, int tileSize, int rows, int cols)
    : Texture(path, std::move(id)), m_TileSize(tileSize), m_Rows(rows), m_Cols(cols) {
        for(int i = 0; i < m_Rows; i++) {
            m_ActiveTileButtons.emplace_back();
            for (int j = 0; j < m_Cols; j++) {
                m_ActiveTileButtons[i].push_back(false);
            }
        }
    }

void TileMap::ClearButtons() {
    for(int i = 0; i < m_Rows; i++) {
        for (int j = 0; j < m_Cols; j++) {
            m_ActiveTileButtons[i][j] = false;
        }
    }
}