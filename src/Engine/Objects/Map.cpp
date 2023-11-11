#include "Map.h"

#include <SDL2/SDL.h>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Engine/Renderer/Renderer.h"

#include "Engine/utils/utils.h"

Map::~Map() {
    for (const auto& row : m_Grid) {
        for (auto *tile : row) {
            delete tile;
        }
    }
}

bool Map::LoadMap(const std::string& filename) {
    std::ifstream file;
    file.open(filename);

    std::string width;
    std::string height;
    std::string src_tile_size;
    std::string dest_tile_size;

    std::getline(file, width);
    if (width.empty()) {
        SDL_Log("Tile map contains no width\n");
        return false;
    }
    std::getline(file, height);
    if (height.empty()) {
        SDL_Log("Tile map contains no height\n");
        return false;
    }
    std::getline(file, src_tile_size);
    if (src_tile_size.empty()) {
        SDL_Log("Tile map contains no source tile size\n");
        return false;
    }
    std::getline(file, dest_tile_size);
    if (dest_tile_size.empty()) {
        SDL_Log("Tile map contains no destination tile size\n");
        return false;
    }
    m_Width = stoi(width);
    m_Height = stoi(height);
    m_SrcTileSize = stoi(src_tile_size);
    m_DestTileSize = stoi(dest_tile_size);

    std::string row;
    std::string entry;
    int i = 0;
    int j = 0;

    while (std::getline(file, row)) {
        std::stringstream ss(row);
        std::vector<Tile*> const row;
        m_Grid.push_back(row);

        SDL_Log("row %d", i);
        while (ss >> entry) {
            if (entry.size() != 2) {
                SDL_Log("Tile map entry has incorrect size");
                return false;
            }
            int const tile_row = (entry[0] - '0');
            int const tile_column = (entry[1] - '0');

            Properties props(m_TextureID, { i, j, m_SrcTileSize, m_SrcTileSize}, {static_cast<float>(j) * m_DestTileSize, static_cast<float>(i) * m_DestTileSize, m_DestTileSize, m_DestTileSize});
            Tile* tile = new Tile(props, tile_row, tile_column, m_SrcTileSize, m_DestTileSize);
            m_Grid[i].push_back(tile);
            SDL_Log("%d: (%d, %d)", j, tile_row * m_SrcTileSize, tile_column * m_SrcTileSize);
            j++;
        }
        j = 0;
        i++;
    }
    return true;
}

void Map::Draw() {
    for (const auto& row : m_Grid) {
        for (auto *tile : row) {
            tile->Draw();
        }
    }
}
