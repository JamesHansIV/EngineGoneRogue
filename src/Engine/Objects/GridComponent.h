#pragma once

#include <utility>
#include <vector>
#include "Engine/Renderer/Renderer.h"
#include "Engine/State/State.h"
#include "SDL2/SDL_rect.h"

/*NOTE:
    * Instances of Box and BoxCounter should be created on the heap using 'new' operator.
*/

const SDL_Color kCountColor = {255, 255, 255, 255};

struct Box {
    Box(const SDL_Rect& srcRect, const DrawColor& boxColor, bool fill)
        : src_rect(srcRect), color(boxColor), should_fill(fill) {
    }

    virtual ~Box() = default;

    SDL_Rect src_rect;
    DrawColor color;
    bool should_fill;
};

struct BoxWithText : public Box {
    BoxWithText(const std::string& text, const SDL_Rect& srcRect, const DrawColor& boxColor, bool fill)
        : Box(srcRect, boxColor, fill), text(text) {
    }

    std::string text;
};

struct BoxWithTexture : public Box {
    BoxWithTexture(std::string textureID, const SDL_Rect& srcRect, const DrawColor& boxColor, bool fill)
        : Box(srcRect, boxColor, fill), TextureID(std::move(textureID)) {
    }

    std::string TextureID;
};

struct BoxWithCounter : public BoxWithTexture {
    BoxWithCounter(int count, const std::string& textureID, const SDL_Rect& srcRect, const DrawColor& boxColor, bool fill)
        : BoxWithTexture(textureID, srcRect, boxColor, fill), count(count) {
    }

    int count;
};

class GridComponent {
public:
    explicit GridComponent(int rows, int columns, int startX, int startY, int cellHeight, int cellWidth)
        : m_rows(rows), m_startX(startX), m_startY(startY), m_columns(columns), m_cellHeight(cellHeight), m_cellWidth(cellWidth) {
        InitializeGrid();
    }

    void AddBox(Box* box, int row, int column) {
        if (row >= 0 && row < m_rows && column >= 0 && column < m_columns) {
            m_Grid[row][column] = box;
        }
    }

    void DrawGrid() {
        Renderer* renderer = Renderer::GetInstance();
        int const offset_x = m_startX;
        int const offset_y = m_startY;

        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_columns; j++) {
                Box* box = m_Grid[i][j];
                if (box != nullptr) {
                    int const box_x = offset_x + j * m_cellWidth;
                    int const box_y = offset_y + i * m_cellHeight;

                    SDL_Rect box_around = {box_x + renderer->GetCameraX(), box_y + renderer->GetCameraY(),
                                            m_cellWidth, m_cellHeight};
                    renderer->DrawRect(box_around, box->color, box->should_fill);

                    if (dynamic_cast<BoxWithTexture*>(box) != nullptr) {
                        DrawTextureOnBox(dynamic_cast<BoxWithTexture*>(box), box_x, box_y);
                    }  
                    if (dynamic_cast<BoxWithText*>(box) != nullptr) {
                        DrawTextOnBox(dynamic_cast<BoxWithText*>(box), box_x, box_y);
                    }  
                    if (dynamic_cast<BoxWithCounter*>(box) != nullptr) {
                        DrawCountOnBox(dynamic_cast<BoxWithCounter*>(box), box_x, box_y);
                    }
                }
            }
        }
    }

    void DrawTextureOnBox(BoxWithTexture* box, int xPos, int yPos) const {
        Renderer* renderer = Renderer::GetInstance();

        SDL_Rect dst_rect = {xPos + renderer->GetCameraX(), yPos + renderer->GetCameraY(),
                                m_cellWidth, m_cellHeight};
        SDL_Rect src_rect = box->src_rect;
        renderer->Draw(box->TextureID, src_rect, dst_rect, 0.0F, nullptr, SDL_FLIP_NONE);
    }

    void DrawTextOnBox(BoxWithText* box, int xPos, int yPos) const {
        Renderer* renderer = Renderer::GetInstance();

        int const text_x = xPos + renderer->GetCameraX() + m_cellWidth / 2;
        int const text_y = yPos + renderer->GetCameraY() + m_cellHeight / 2;

        Texture* text_texture = renderer->AddTextTexture("text", box->text, kCountColor);
        SDL_Rect src_rect = {0, 0, text_texture->GetWidth(), text_texture->GetHeight()};
        int const text_texture_x = text_x - text_texture->GetWidth() / 2;
        int const text_texture_y = text_y - text_texture->GetHeight() / 2;
        SDL_Rect text_texture_rect = {text_texture_x, text_texture_y, text_texture->GetWidth(), text_texture->GetHeight()};
        renderer->Draw("text", src_rect, text_texture_rect, SDL_FLIP_NONE);
    }

    void DrawCountOnBox(Box* box, int xPos, int yPos) const {
        auto* box_with_counter = dynamic_cast<BoxWithCounter*>(box);
        int const count = box_with_counter->count;

        Renderer* renderer = Renderer::GetInstance();

        int const count_x = xPos + renderer->GetCameraX() + m_cellWidth - 20;
        int const count_y = renderer->GetCameraY() + yPos;
        int const count_width = 20;
        int const count_height = 20;

        SDL_Rect count_rect = {count_x, count_y, count_width, count_height};
        renderer->DrawRect(count_rect, box->color, true);

        Texture* count_texture = renderer->AddTextTexture("count", std::to_string(count), kCountColor);
        SDL_Rect src_rect = {0, 0, count_texture->GetWidth(), count_texture->GetHeight()};
        int const count_texture_x = count_x + (count_width - count_texture->GetWidth()) / 2;
        int const count_texture_y = count_y + (count_height - count_texture->GetHeight()) / 2;
        SDL_Rect count_texture_rect = {count_texture_x, count_texture_y, count_texture->GetWidth(), count_texture->GetHeight()};
        renderer->Draw("count", src_rect, count_texture_rect, SDL_FLIP_NONE);
    }

     ~GridComponent() {
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_columns; j++) {
                if (m_Grid[i][j] != nullptr){
                    delete m_Grid[i][j];
                } 
            }
        }
    }


private:
    std::vector<std::vector<Box*>> m_Grid;
    int m_startX;
    int m_startY;
    int m_rows;
    int m_columns;
    int m_cellHeight;
    int m_cellWidth;

    [[nodiscard]] int GetTotalWidth() const {
    return m_columns * m_cellWidth;
    }

    [[nodiscard]] int GetTotalHeight() const {
        return m_rows * m_cellHeight;
    }

    void InitializeGrid() {
        m_Grid = std::vector<std::vector<Box*>>(m_rows, std::vector<Box*>(m_columns, nullptr));
    }
};