#include "Map.h"

#include <SDL2/SDL.h>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include "../Graphics/TextureManager.h"

#include "../utils.h"

Map::~Map() {
    if (!m_Grid) {
        return;
    }
    for (int i = 0; i < m_Height; i++) {
        delete m_Grid[i];
    }
    delete m_Grid;
}

bool Map::LoadMap(std::string filename) {
    std::ifstream file;
    file.open(filename);

    std::string width = "";
    std::string height = "";
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
    m_Width = atoi(width.c_str());
    m_Height = atoi(height.c_str());

    std::string row;
    std::string entry;
    int i = 0;
    int j = 0;
    m_Grid = new Coord*[m_Height];
    while (std::getline(file, row)) {
        std::stringstream ss(row);
        m_Grid[i] = new Coord[m_Width];
        SDL_Log("row %d", i);
        while (ss >> entry) {
            if (entry.size() != 2) {
                SDL_Log("Tile map entry has incorrect size");
                return false;
            }
            
            Coord c = { int(entry[0] - '0'), int(entry[1] - '0') };
            m_Grid[i][j] = c;
            SDL_Log("%d: (%d, %d)", j, c.x, c.y);
            j++;
        }
        j = 0;
        i++;
    }
    return true;
}

void Map::Draw() {
    int destTileWidth = SCREEN_WIDTH / m_Width;
    int destTileHeight = SCREEN_HEIGHT / m_Height;

    for (int i = 0; i < m_Height; i++) {
        for (int j = 0; j < m_Width; j++) {
            //TextureManager::GetInstance()->DrawFrame(m_TextureID, i * destTileHeight, j * destTileHeight, dest);
        }
    }
}