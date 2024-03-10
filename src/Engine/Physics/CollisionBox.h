#pragma once
#include "Engine/utils/utils.h"
#include "SDL2/SDL.h"

class CollisionBox {
   public:
    CollisionBox() {
        // Collision boxes are 2 pixels smaller on every side by default
        m_Buffer = {2, 2, 4, 4};
    }

    inline Rect GetRect() { return m_Box; }

    inline Rect GetPrevRect() { return m_PrevBox; }

    inline void SetCorrection(float x, float y, int w, int h) {
        m_Buffer = {x, y, w, h};
    }

    void Set(float x, float y, int w, int h) {
        m_PrevBox = m_Box;
        m_Box = {x - m_Buffer.x, y - m_Buffer.y, w - m_Buffer.w,
                 h - m_Buffer.h};
    }

    void clear() { m_Box = {}; }

   private:
    Rect m_Box;
    Rect m_PrevBox;
    Rect m_Buffer;
};
