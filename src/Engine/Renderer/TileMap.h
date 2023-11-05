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
        TileMap(const std::string& path, std::string textureID, int tileSize, int rows, int cols)
            : Texture(path, textureID), m_TileSize(tileSize), m_Rows(rows), m_Cols(cols) {}
        TileMap(const char* path, std::string id, int tileSize, int rows, int cols)
            : Texture(path, id), m_TileSize(tileSize), m_Rows(rows), m_Cols(cols) {}

        inline int GetTileSize() { return m_TileSize; }
        inline int GetCols() { return m_Rows; }
        inline int GetRows() { return m_Cols; }

    private:
        int m_TileSize;
        int m_Rows;
        int m_Cols;
};