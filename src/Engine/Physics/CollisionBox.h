#pragma once
#include "Engine/utils/utils.h"
#include "SDL2/SDL.h"

class CollisionBox {
   public:
    CollisionBox() {
        // Collision boxes are 2 pixels smaller on every side by default
        m_buffer = {6, 6, 12, 12};
    }

    inline Rect GetRect() { return m_box; }

    inline Rect GetPrevRect() { return m_prev_box; }

    inline void SetCorrection(float x, float y, int w, int h) {
        m_buffer = {x, y, w, h};
    }

    void Set(float x, float y, int w, int h) {
        m_prev_box = m_box;
        m_box = {x + m_buffer.x, y + m_buffer.y, w - m_buffer.w,
                 h - m_buffer.h};
    }

    void Clear() { m_box = {}; }

   private:
    Rect m_box;
    Rect m_prev_box;
    Rect m_buffer;
};
