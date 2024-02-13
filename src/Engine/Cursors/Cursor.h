#pragma once
#include<cstdlib>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>

enum class CursorType {
    POINT = 0,
    DRAW,
    ERASE
};


class Cursor {
    public:
        Cursor();
        ~Cursor();

        int x;
        int y;

        void Draw();
        void SetCursor(CursorType cusor_type);
        void SetCursor(int edit_mode);

        SDL_Rect UpdateAndGetRect();
        std::string GetTextureId(int edit_mode);



    private:
        SDL_Rect m_Cursor_Rect;
        int m_Width;
        int m_Height;
        double m_Scale;
        CursorType m_Cursor_Type;

        std::unordered_map<int, CursorType>edit_mode_to_cursor_type_map{
            {0, CursorType::POINT}, 
            {1, CursorType::DRAW}, 
            {2, CursorType::ERASE},
            {3, CursorType::POINT}
        };

        std::unordered_map<CursorType, std::string>cursor_type_to_texture_id_map {
            {CursorType::POINT, "editor-cursor-point"},
            {CursorType::DRAW, "editor-cursor-draw"},
            {CursorType::ERASE, "editor-cursor-erase"}
        };
};