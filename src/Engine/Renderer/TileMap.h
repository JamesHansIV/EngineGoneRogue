#pragma once
#include <vector>
#include "Texture.h"

struct TilePos {
    int row;
    int col;
    int w;
    int h;
};

class TileMap : public Texture {
   public:
    TileMap(const std::string& path, std::string textureID, int tileSize,
            int rows, int cols);
    TileMap(const char* path, std::string id, int tileSize, int rows, int cols);

    [[nodiscard]] inline int GetTileSize() const { return m_tile_size; }

    [[nodiscard]] inline int GetRows() const { return m_rows; }

    [[nodiscard]] inline int GetCols() const { return m_cols; }

    std::vector<std::vector<bool>>& GetActiveButtons() {
        return m_active_tile_buttons;
    }

    void ClearButtons();

   private:
    int m_tile_size;
    int m_rows;
    int m_cols;
    std::vector<std::vector<bool>> m_active_tile_buttons;
};