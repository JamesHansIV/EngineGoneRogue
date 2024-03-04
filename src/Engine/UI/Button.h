#pragma once

#include "Engine/Application/Application.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/utils/utils.h"
#include "Engine/Physics/Vector2D.h"
#include "Engine/utils/utils.h"
#include "SDL2/SDL_rect.h"

enum ButtonState {
    BUTTON_STATE_NORMAL = 0,
    BUTTON_STATE_HOVER = 1,
    BUTTON_STATE_PRESSED = 2,
};

// Maybe inherit from Game Object?
class Button {
   public:
    Button(SDL_Rect rect, const std::string text, void (*callback)())
        : m_shape(rect) {
        m_callback = callback;
        m_text = text;
    };

    ~Button() = default;

    void Draw() {

        Renderer* renderer = Renderer::GetInstance();
        const Position pos{renderer->GetCameraX() - m_shape.w +
                               Application::Get()->GetWindowWidth(),
                           renderer->GetCameraY()};

        m_shape = DrawBar(pos, Size{m_shape.w, m_shape.h});

        switch (m_state) {
            case BUTTON_STATE_NORMAL:
                renderer->DrawRect(m_shape, {0, 0, 0, 255}, true);
                break;
            case BUTTON_STATE_HOVER:
                renderer->DrawRect(m_shape, {0, 255, 0, 255}, true);
                break;
            case BUTTON_STATE_PRESSED:
                renderer->DrawRect(m_shape, {0, 0, 0, 235}, true);
                break;
        }
    };

    void Update() {
        Vector2D const mouse_pos{static_cast<float>(InputChecker::GetMouseX()),
                                 static_cast<float>(InputChecker::GetMouseY())};
        if (mouse_pos.X > m_shape.x && mouse_pos.X < m_shape.x + m_shape.w &&
            mouse_pos.Y > m_shape.y && mouse_pos.Y < m_shape.y + m_shape.h) {
            m_state = BUTTON_STATE_HOVER;
            if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
                m_state = BUTTON_STATE_PRESSED;
                m_callback();
            }
        } else {
            m_state = BUTTON_STATE_NORMAL;
        }
    };

   private:
    SDL_Rect m_shape;
    std::string m_text;
    ButtonState m_state;
    void (*m_callback)();
};
