#pragma once

#include "GameObject.h"

class Tile : public GameObject {
    public:
        Tile(Properties& props, int tileRow, int tileColumn, int srcTileSize, int destTileSize)
            : GameObject(props), m_TileRow(tileRow), m_TileColumn(tileColumn), m_SrcTileSize(srcTileSize), m_DestTileSize(destTileSize) {}

        void Draw() override;
        void Clean() override {};
        void Update(float dt) override {};

        int GetRow() const { return m_TileRow; }
        int GetColumn() const { return m_TileColumn; }

    private:
        int m_SrcTileSize;
        int m_DestTileSize;
        int m_TileRow;
        int m_TileColumn;
};