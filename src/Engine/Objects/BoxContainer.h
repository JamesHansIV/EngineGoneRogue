#pragma once

#include <vector>
#include "Engine/Renderer/Renderer.h"
#include "Engine/State/State.h"
#include "SDL2/SDL_rect.h"

struct Box{ 
    std::string TextureID;
    SDL_Rect src_rect;
    DrawColor color;
};

class BoxContainer{
    public:
        explicit BoxContainer(int startX, int startY, int height, int boxWidth, int spacing)
            : m_startX(startX), m_StartY(startY), m_Height(height), m_BoxWidth(boxWidth), m_Spacing(spacing){
        }

        void AddBox(const Box& box) {
            m_Boxes.push_back(box);
        }

        void DrawBoxes() {
            Renderer* renderer = Renderer::GetInstance();
            int x = m_startX;
            for (Box& box : m_Boxes) {
                SDL_Rect dst_rect = {x + renderer->GetCameraX(), renderer->GetCameraY() + m_StartY, m_BoxWidth,
                        m_Height};
                SDL_Rect src_rect = box.src_rect;
                renderer->Draw(box.TextureID, src_rect, dst_rect, 0.0F, nullptr, SDL_FLIP_NONE);
                
                SDL_Rect box_around = {x + renderer->GetCameraX(), renderer->GetCameraY(), m_BoxWidth,
                        40};
                renderer->DrawRect(box_around, box.color);
                x += m_BoxWidth + m_Spacing;
            }
            SDL_Rect box = {renderer->GetCameraX() + m_startX, renderer->GetCameraY() + m_StartY, GetTotalWidth(), m_Height};
            renderer->DrawRect(box, {0, 0, 0, 255});
        }

    private:
        std::vector<Box> m_Boxes;
        int m_startX;
        int m_StartY;
        int m_Height;
        int m_BoxWidth;
        int m_Spacing;

        int GetTotalWidth() { return m_BoxWidth * static_cast<int>(m_Boxes.size());};
};