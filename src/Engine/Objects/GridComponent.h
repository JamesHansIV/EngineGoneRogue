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

enum TextAlign{
    KLeft,
    KCenter,
    KRight   
};

struct Box {
    Box(const SDL_Rect& srcRect, const DrawColor& boxColor, bool fill)
        : src_rect(srcRect), color(boxColor), should_fill(fill) {
    }

    virtual ~Box() = default;

    SDL_Rect src_rect;
    DrawColor color;
    bool should_fill;
    int row;
    int col;
};

struct BoxWithText : public Box {
    BoxWithText(const std::string& text, const SDL_Rect& srcRect, const DrawColor& boxColor, bool fill, TextAlign alignment)
        : Box(srcRect, boxColor, fill), text(text), text_align(alignment) {
    }

    std::string text;
    TextAlign text_align;
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
        : m_rows(rows), m_start_x(startX), m_start_y(startY), m_columns(columns), m_cell_height(cellHeight), m_cell_width(cellWidth) {
        InitializeGrid();
        m_column_widths = std::vector<int>(m_columns, m_cell_width);
    }

    void AddBox(Box* box, int row, int column) {
        if (row >= 0 && row < m_rows && column >= 0 && column < m_columns) {
            m_grid[row][column] = box;
            box->row = row;
            box->col = column;
        }
    }
    void SetColumnWidth(int column, int width) {
        if (column >= 0 && column < m_columns) {
            m_column_widths[column] = width;
        }
    }

    int GetColumnWidth(int column) const {
        if (column >= 0 && column < m_columns) {
            return m_column_widths[column];
        }
        return m_cell_width;
    }

    int GetColumnStartX(int column) const {
        int start_x = m_start_x;
        for (int i = 0; i < column; i++) {
            start_x += GetColumnWidth(i);
        }
        return start_x;
    }

    void DrawGrid() {
        Renderer* renderer = Renderer::GetInstance();
        int const offset_x = m_start_x;
        int const offset_y = m_start_y;

        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_columns; j++) {
                Box* box = m_grid[i][j];
                if (box != nullptr) {
                    int const box_x = offset_x + GetColumnStartX(j);
                    int const box_y = offset_y + i * m_cell_height;

                    SDL_Rect box_around = {box_x + renderer->GetCameraX(), box_y + renderer->GetCameraY(),
                                            m_column_widths[j], m_cell_height};
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
                                m_column_widths[box->col], m_cell_height};
        SDL_Rect src_rect = box->src_rect;
        renderer->Draw(box->TextureID, src_rect, dst_rect, 0.0F, nullptr, SDL_FLIP_NONE);
    }

    void DrawTextOnBox(BoxWithText* box, int xPos, int yPos) const {
        Renderer* renderer = Renderer::GetInstance();

        int const text_x = xPos + renderer->GetCameraX() + (m_column_widths[box->col]) / 2;
        int const text_y = yPos + renderer->GetCameraY() + m_cell_height / 2;

        Texture* text_texture = renderer->AddTextTexture("text", box->text, kCountColor);
        SDL_Rect src_rect = {0, 0, text_texture->GetWidth(), text_texture->GetHeight()};

        // Calculate the text position based on the text alignment
        int text_texture_x = text_x - text_texture->GetWidth() / 2; // Ce   nter alignment by default

        if (box->text_align == KLeft) {
            text_texture_x = xPos + renderer->GetCameraX();
        } else if (box->text_align == KRight) {
            text_texture_x = xPos + renderer->GetCameraX() + m_column_widths[box->col] - text_texture->GetWidth();
        }

        int const text_texture_y = text_y - text_texture->GetHeight() / 2;
        SDL_Rect text_texture_rect = {text_texture_x, text_texture_y, text_texture->GetWidth(), text_texture->GetHeight()};
        renderer->Draw("text", src_rect, text_texture_rect, SDL_FLIP_NONE);
    }

    void DrawCountOnBox(Box* box, int xPos, int yPos) const {
        auto* box_with_counter = dynamic_cast<BoxWithCounter*>(box);
        int const count = box_with_counter->count;

        Renderer* renderer = Renderer::GetInstance();

        int const count_x = xPos + renderer->GetCameraX() + m_column_widths[box->col] - 20;
        int const count_y = yPos + renderer->GetCameraY();
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
                if (m_grid[i][j] != nullptr){
                    delete m_grid[i][j];
                } 
            }
        }
    }


private:
    std::vector<std::vector<Box*>> m_grid;
    std::vector<int> m_column_widths;
    int m_start_x;
    int m_start_y;
    int m_rows;
    int m_columns;
    int m_cell_height;
    int m_cell_width;

    [[nodiscard]] int GetTotalWidth() const {
    return m_columns * m_cell_width;
    }

    [[nodiscard]] int GetTotalHeight() const {
        return m_rows * m_cell_height;
    }

    void InitializeGrid() {
        m_grid = std::vector<std::vector<Box*>>(m_rows, std::vector<Box*>(m_columns, nullptr));
    }
};