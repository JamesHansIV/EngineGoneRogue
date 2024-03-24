#pragma once
#include<cstdlib>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>

#include "Engine/Editor/EditMode.h"

enum class CursorType {
    POINT = 0,
    DRAW,
    ERASE,
    MULTI_SELECT,
    DRAG_MOVE,
    PAINT_BUCKET
};


class Cursor {
    public:
        Cursor();
        ~Cursor();

        int x;
        int y;

        void Draw();
        void SetCursor(CursorType cursor_type);
        void SetCursor(EditMode edit_mode);

        SDL_Rect UpdateAndGetRect();
        SDL_Rect UpdateAndGetRect(int offsetX, int offsetY);
        std::string GetTextureId(int edit_mode);

        CursorType GetCursorType() {return m_Cursor_Type;}



    private:
        SDL_Rect m_Cursor_Rect;
        int m_Width;
        int m_Height;
        double m_Scale;
        CursorType m_Cursor_Type;

        std::unordered_map<EditMode, CursorType>edit_mode_to_cursor_type_map {
            {EditMode::NONE, CursorType::POINT}, 
            {EditMode::DRAW, CursorType::DRAW}, 
            {EditMode::ERASE, CursorType::ERASE},
            {EditMode::OBJECT_SELECT, CursorType::POINT},
            {EditMode::TILE_SELECT, CursorType::MULTI_SELECT},
            {EditMode::LAYER_SELECT, CursorType::POINT},
            {EditMode::DRAG_MOVE, CursorType::DRAG_MOVE},
            {EditMode::PAINT_BUCKET, CursorType::PAINT_BUCKET}
        };

        std::unordered_map<CursorType, std::string>cursor_type_to_texture_id_map {
            {CursorType::POINT, "editor-cursor-point"},
            {CursorType::DRAW, "editor-cursor-draw"},
            {CursorType::ERASE, "editor-cursor-erase"},
            {CursorType::MULTI_SELECT, "editor-cursor-multi-select"},
            {CursorType::DRAG_MOVE, "editor-cursor-drag-move"},
            {CursorType::PAINT_BUCKET, "editor-cursor-paint-bucket"}
        };
};