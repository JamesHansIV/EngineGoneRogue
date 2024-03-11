#include "TileMap.h"

#include <utility>

TileMap::TileMap(const std::string& path, std::string textureID, int tileSize,
                 int rows, int cols)
    : Texture(path, std::move(textureID)),
      m_tile_size(tileSize),
      m_rows(rows),
      m_cols(cols) {

    for (int i = 0; i < m_rows; i++) {
        m_active_tile_buttons.emplace_back();
        for (int j = 0; j < m_cols; j++) {
            m_active_tile_buttons[i].push_back(false);
        }
    }
}

TileMap::TileMap(const char* path, std::string id, int tileSize, int rows,
                 int cols)
    : Texture(path, std::move(id)),
      m_tile_size(tileSize),
      m_rows(rows),
      m_cols(cols) {
    for (int i = 0; i < m_rows; i++) {
        m_active_tile_buttons.emplace_back();
        for (int j = 0; j < m_cols; j++) {
            m_active_tile_buttons[i].push_back(false);
        }
    }
}

void TileMap::ClearButtons() {
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            m_active_tile_buttons[i][j] = false;
        }
    }
}