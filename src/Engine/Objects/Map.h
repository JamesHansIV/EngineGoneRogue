#pragma once

#include <string>
#include <vector>
#include "Tile.h"


class Map {
    public:
        Map(std::string textureID) : m_TextureID(textureID), m_Width(0), m_Height(0) { }
        ~Map();

        bool LoadMap(std::string filename);
        void Draw();

    private:
        std::string m_TextureID;
        int m_Width;
        int m_Height;
        int m_SrcTileSize;
        int m_DestTileSize;
        std::vector< std::vector<Tile*>> m_Grid;
};