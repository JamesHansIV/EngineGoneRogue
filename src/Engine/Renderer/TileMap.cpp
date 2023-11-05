#include "TileMap.h"

TileMap::TileMap(const std::string& path, std::string textureID, int tileSize, int rows, int cols)
    : Texture(path, textureID), m_TileSize(tileSize), m_Rows(rows), m_Cols(cols) {}
    
TileMap::TileMap(const char* path, std::string id, int tileSize, int rows, int cols)
    : Texture(path, id), m_TileSize(tileSize), m_Rows(rows), m_Cols(cols) {}