#pragma once

#include "Engine/Application/Application.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/utils/utils.h"
#include "Engine/Physics/Vector2D.h"
#include "Engine/Renderer/DrawElement.h"
#include "Engine/utils/utils.h"
#include "SDL2/SDL_rect.h"

const TilePos kDefaultNormal = {0, 0, 24, 16};
const TilePos kDefaultHover = {0, 1, 24, 16};

enum ButtonState {
    ButtonStateNormal = 0,
    ButtonStateHover = 1,
    ButtonStatePressed = 2,
};

// Maybe inherit from Game Object?
class Button {
   public:
    Button() = default;

    Button(std::string textureID, SDL_Rect rect, const std::string& text,
           void (*callback)())
        : m_shape(rect),
          m_normal_src(kDefaultNormal),
          m_hover_src(kDefaultHover) {
        m_relative_pos = Position{rect.x, rect.y};
        m_callback = callback;
        m_text = text;
        Renderer::GetInstance()->AddTextTexture(m_text, m_text, {0, 0, 0, 255});
        m_draw_element = DrawElement(textureID, rect);
    };

    ~Button() {}

    void Draw() {
        Renderer* renderer = Renderer::GetInstance();

        const Position pos{m_relative_pos.x, m_relative_pos.y};
        Texture* text_texture = Renderer::GetInstance()->GetTexture(m_text);
        SDL_Rect src_rect = {0, 0, text_texture->GetWidth(),
                             text_texture->GetHeight()};

        SDL_Rect text_shape = {
            m_shape.x + m_shape.w / 2 - text_texture->GetWidth() / 2,
            m_shape.y + m_shape.h / 2 - text_texture->GetHeight() / 2,
            text_texture->GetWidth(), text_texture->GetHeight()};

        switch (m_state) {
            case ButtonStateNormal:
                //renderer->DrawRectRelative(m_shape, {0, 0, 0, 255}, true);
                m_draw_element.Draw(m_normal_src);
                break;
            case ButtonStateHover:
                //renderer->DrawRectRelative(m_shape, {0, 255, 0, 255}, true);
                m_draw_element.Draw(m_hover_src);
                break;
            case ButtonStatePressed:
                //renderer->DrawRectRelative(m_shape, {0, 0, 0, 235}, true);
                m_draw_element.Draw(m_normal_src);
                break;
        }
        renderer->DrawRelative(m_text, src_rect, text_shape);
    };

    void Update() {
        Vector2D const mouse_pos{static_cast<float>(InputChecker::GetMouseX()),
                                 static_cast<float>(InputChecker::GetMouseY())};
        if (mouse_pos.X > m_shape.x && mouse_pos.X < m_shape.x + m_shape.w &&
            mouse_pos.Y > m_shape.y && mouse_pos.Y < m_shape.y + m_shape.h) {
            m_state = ButtonStateHover;
            if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
                m_state = ButtonStatePressed;
                m_callback();
            }
        } else {
            m_state = ButtonStateNormal;
        }
    };

    void HandleEvent(MouseButtonDownEvent* event) {
        int mouse_x = event->GetX();
        int mouse_y = event->GetY();
        if (mouse_x > m_shape.x && mouse_x < m_shape.x + m_shape.w &&
            mouse_y > m_shape.y && mouse_y < m_shape.y + m_shape.h) {
            m_callback();
        }
    }

   private:
    std::string m_texture_id;
    DrawElement m_draw_element;
    TilePos m_normal_src;
    TilePos m_hover_src;
    Position m_relative_pos;
    SDL_Rect m_shape;
    std::string m_text;
    ButtonState m_state;
    void (*m_callback)();
};
