#pragma once
#include "SDL2/SDL.h"

class CollisionBox {
   public:
    CollisionBox() {}

    inline SDL_Rect GetRect() { return m_Box; }

    inline SDL_Rect GetPrevRect() { return m_PrevBox; }

    inline void SetCorrection(int x, int y, int w, int h) {
        m_Buffer = {x, y, w, h};
    }

    void Set(int x, int y, int w, int h) {
        m_PrevBox = m_Box;
        m_Box = {x - m_Buffer.x, y - m_Buffer.y, w - m_Buffer.w,
                 h - m_Buffer.h};
    }

    void clear() { m_Box = {}; }

   private:
    SDL_Rect m_Box;
    SDL_Rect m_PrevBox;
    SDL_Rect m_Buffer;
};
