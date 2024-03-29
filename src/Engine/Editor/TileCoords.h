#pragma once

#include "Engine/utils/utils.h"

#include <vector>

struct TileCoords {
    int row, col;

    TileCoords(int r=0, int c=0) : row(r), col(c) {}

    bool IsInBounds() {
        int levelRows = (int)LevelHeight / (int)TileSize;
        int levelCols = (int)LevelWidth / (int)TileSize;
        return !(row < 0 || col < 0 || row >= levelRows || col >= levelRows);
    }

    bool operator()(const TileCoords& coords) const {
        return std::hash<int>()(coords.row) ^ std::hash<int>()(coords.col);
    }

    TileCoords operator-(const TileCoords& rhs) const {
        return TileCoords(row - rhs.row, col - rhs.col);
    }

    TileCoords operator+(const TileCoords& rhs) const {
        return TileCoords(row + rhs.row, col + rhs.col);
    }

    bool operator==(const TileCoords& rhs) const {
        return (row == rhs.row && col == rhs.col);
    }

    bool operator!=(const TileCoords& rhs) const {
        return !(row == rhs.row && col == rhs.col);
    }
};