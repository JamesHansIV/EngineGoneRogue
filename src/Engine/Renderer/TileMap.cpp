#include "TileMap.h"

TileMap::TileMap(const std::string& path, std::string textureID, int tileSize, int rows, int cols)
    : Texture(path, textureID), m_TileSize(tileSize), m_Rows(rows), m_Cols(cols) {

        for(int i = 0; i < m_Rows; i++) {
            m_ActiveTileButtons.push_back(std::vector<bool>());
            for (int j = 0; j < m_Cols; j++) {
                m_ActiveTileButtons[i].push_back(false);
            }
        }
    }
    
TileMap::TileMap(const char* path, std::string id, int tileSize, int rows, int cols)
    : Texture(path, id), m_TileSize(tileSize), m_Rows(rows), m_Cols(cols) {
        for(int i = 0; i < m_Rows; i++) {
            m_ActiveTileButtons.push_back(std::vector<bool>());
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