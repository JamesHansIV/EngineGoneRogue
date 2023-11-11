#pragma once

#include "Tile.h"
#include <string>
#include <utility>
#include <vector>


class Map {
    public:
        explicit Map(std::string textureID) : m_TextureID(std::move(std::move(textureID))) { }
        ~Map();

        bool LoadMap(const std::string& filename);
        void Draw();

    private:
        std::string m_TextureID;
        int m_Width{0};
        int m_Height{0};
        int m_SrcTileSize;
        int m_DestTileSize;
        std::vector< std::vector<Tile*>> m_Grid;
};