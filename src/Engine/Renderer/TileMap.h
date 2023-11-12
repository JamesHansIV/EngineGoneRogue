#pragma once
#include "Texture.h"
#include <vector>

struct TilePos {
    int row;
    int col;
    int w;
    int h;
};

class TileMap : public Texture{
    public:
        TileMap(const std::string& path, std::string textureID, int tileSize, int rows, int cols);
        TileMap(const char* path, std::string id, int tileSize, int rows, int cols);

        inline int GetTileSize() const { return m_TileSize; }
        inline int GetRows() const { return m_Rows; }
        inline int GetCols() const { return m_Cols; }
        std::vector<std::vector<bool>>& GetActiveButtons() { return m_ActiveTileButtons; }
        void ClearButtons();

    private:
        int m_TileSize;
        int m_Rows;
        int m_Cols;
        std::vector< std::vector<bool>> m_ActiveTileButtons;
};