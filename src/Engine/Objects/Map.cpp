#include "Map.h"

#include <SDL2/SDL.h>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include "Engine/Renderer/Renderer.h"

#include "utils.h"

Map::~Map() {
    for (auto row : m_Grid) {
        for (auto tile : row) {
            delete tile;
        }
    }
}

bool Map::LoadMap(std::string filename) {
    std::ifstream file;
    file.open(filename);

    std::string width = "";
    std::string height = "";
    std::string srcTileSize = "";
    std::string destTileSize = "";

    std::getline(file, width);
    if (width == "") {
        SDL_Log("Tile map contains no width\n");
        return false;
    }
    std::getline(file, height);
    if (height == "") {
        SDL_Log("Tile map contains no height\n");
        return false;
    }
    std::getline(file, srcTileSize);
    if (srcTileSize == "") {
        SDL_Log("Tile map contains no source tile size\n");
        return false;
    }
    std::getline(file, destTileSize);
    if (destTileSize == "") {
        SDL_Log("Tile map contains no destination tile size\n");
        return false;
    }
    m_Width = stoi(width);
    m_Height = stoi(height);
    m_SrcTileSize = stoi(srcTileSize);
    m_DestTileSize = stoi(destTileSize);

    std::string row;
    std::string entry;
    int i = 0;
    int j = 0;

    while (std::getline(file, row)) {
        std::stringstream ss(row);
        std::vector<Tile*> row;
        m_Grid.push_back(row);

        SDL_Log("row %d", i);
        while (ss >> entry) {
            if (entry.size() != 2) {
                SDL_Log("Tile map entry has incorrect size");
                return false;
            }
            int tileRow = int(entry[0] - '0');
            int tileColumn = int(entry[1] - '0');

            Properties props(m_TextureID, j * m_DestTileSize, i * m_DestTileSize, m_DestTileSize, m_SrcTileSize);
            Tile* tile = new Tile(props, tileRow, tileColumn, m_SrcTileSize, m_DestTileSize);
            m_Grid[i].push_back(tile);
            SDL_Log("%d: (%d, %d)", j, tileRow * m_SrcTileSize, tileColumn * m_SrcTileSize);
            j++;
        }
        j = 0;
        i++;
    }
    return true;
}

void Map::Draw() {
    for (auto row : m_Grid) {
        for (auto tile : row) {
            tile->Draw();
        }
    }
}