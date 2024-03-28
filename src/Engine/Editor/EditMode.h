#pragma once

enum class EditMode { 
    NONE = 0, 
    DRAW, 
    ERASE,
    SELECT, 
    OBJECT_SELECT, 
    TILE_SELECT, 
    LAYER_SELECT,
    DRAG_MOVE,
    PAINT_BUCKET,
    TEMP_MULTI_SELECT
};