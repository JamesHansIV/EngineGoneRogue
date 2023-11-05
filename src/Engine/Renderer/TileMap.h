#pragma once
#include "Texture.h"

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

        inline int GetTileSize() { return m_TileSize; }
        inline int GetRows() { return m_Rows; }
        inline int GetCols() { return m_Cols; }

    private:
        int m_TileSize;
        int m_Rows;
        int m_Cols;
};