#pragma once
#include<cstdlib>
#include<Engine/Animation/Animation.h>

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
        
        std::string GetID() const { return m_ID; }
        std::string GetFilePath() const { return m_FilePath; }



    private:
        std::string m_ID;
        std::string m_FilePath;
        // SDL_Texture* m_Texture;
        std::string m_TextrueID;
        int m_Width;
        int m_Height;
        float m_Scale;
        CursorType m_Cursor_Type;
        Animation* m_Animation;

        std::unordered_map<int, CursorType>edit_mode_to_cursor_type_map{
            {0, CursorType::POINT}, 
            {1, CursorType::DRAW}, 
            {2, CursorType::ERASE},
            {3, CursorType::POINT}
        };
};

// load texture in editor