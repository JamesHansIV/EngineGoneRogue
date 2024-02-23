#pragma once

#include <vector>
#include "Engine/Renderer/Renderer.h"
#include "Engine/State/State.h"
#include "SDL2/SDL_rect.h"

const SDL_Color kCountColor = {255, 255, 255, 255};

struct Box {
    Box(const std::string& textureID, const SDL_Rect& srcRect, const DrawColor& boxColor, bool fill)
        : TextureID(textureID), src_rect(srcRect), color(boxColor), should_fill(fill) {
    }

    virtual ~Box() = default;

    std::string TextureID;
    SDL_Rect src_rect;
    DrawColor color;
    bool should_fill;
};

struct BoxWithCounter : public Box {
    BoxWithCounter(const std::string& textureID, const SDL_Rect& srcRect, const DrawColor& boxColor, bool fill, int itemCount)
        : Box(textureID, srcRect, boxColor, fill), count(itemCount) {
    }
    int count;
};

class BoxContainer {
public:
    explicit BoxContainer(int startX, int startY, int height, int boxWidth, int spacing)
        : m_startX(startX), m_StartY(startY), m_Height(height), m_BoxWidth(boxWidth), m_Spacing(spacing) {
    }

    void AddBox(Box* box) {
        m_Boxes.push_back(box);
    }

    void DrawBoxes() {
        Renderer* renderer = Renderer::GetInstance();
        int offset = m_startX;
        for (Box* box : m_Boxes) {

            SDL_Rect box_around = {offset + renderer->GetCameraX(), renderer->GetCameraY() + m_StartY, m_BoxWidth,
                                    40};
            renderer->DrawRect(box_around, box->color, box->should_fill);

            SDL_Rect dst_rect = {offset + renderer->GetCameraX(), renderer->GetCameraY() + m_StartY, m_BoxWidth,
                                    m_Height};
            SDL_Rect src_rect = box->src_rect;
            renderer->Draw(box->TextureID, src_rect, dst_rect, 0.0F, nullptr, SDL_FLIP_NONE);

            if (dynamic_cast<BoxWithCounter*>(box) != nullptr) {
                DrawCount(box, offset);
            }
            offset += m_BoxWidth + m_Spacing;
        }
    }

    void DrawCount(Box* box, int xPos) const {
        auto* box_with_counter = dynamic_cast<BoxWithCounter*>(box);
        int const count = box_with_counter->count;

        Renderer* renderer = Renderer::GetInstance();

        int count_x = xPos + renderer->GetCameraX() + m_BoxWidth - 20;
        int count_y = renderer->GetCameraY() + m_StartY;
        int count_width = 20;
        int count_height = 20;

        SDL_Rect count_rect = {count_x, count_y, count_width, count_height};
        renderer->DrawRect(count_rect, box->color, true);

        Texture* count_texture = renderer->AddTextTexture("count", std::to_string(count), kCountColor);
        SDL_Rect src_rect = {0, 0, count_texture->GetWidth(), count_texture->GetHeight()};
        int const count_texture_x = count_x + (count_width - count_texture->GetWidth()) / 2;
        int const count_texture_y = count_y + (count_height - count_texture->GetHeight()) / 2;
        SDL_Rect count_texture_rect = {count_texture_x, count_texture_y, count_texture->GetWidth(), count_texture->GetHeight()};
        renderer->Draw("count", src_rect, count_texture_rect, SDL_FLIP_NONE);
    }

    ~BoxContainer() {
        for (Box* box : m_Boxes) {
            delete box;
        }
        m_Boxes.clear();
    }

private:
    std::vector<Box*> m_Boxes;
    int m_startX;
    int m_StartY;
    int m_Height;
    int m_BoxWidth;
    int m_Spacing;

    int GetTotalWidth() { return m_BoxWidth * static_cast<int>(m_Boxes.size()); };
};