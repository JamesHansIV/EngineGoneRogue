#include "Cursor.h"
#include <iostream>
#include "Engine/Application/Application.h"
#include "imgui.h"

Cursor::Cursor() {
    m_width = 24;
    m_height = 24;
    m_scale = 1;
    m_cursor_type = CursorType::POINT;
}

Cursor::~Cursor() = default;

SDL_Rect Cursor::UpdateAndGetRect() {
    ImGuiIO const& io = ImGui::GetIO();

    m_cursor_rect.h = m_height * m_scale;
    m_cursor_rect.w = m_width * m_scale;
    m_cursor_rect.x = io.MousePos.x;
    m_cursor_rect.y = io.MousePos.y - m_cursor_rect.h;

    return m_cursor_rect;
}

std::string Cursor::GetTextureId(int edit_mode) {
    if (edit_mode == 3) {
        edit_mode = 0;
    }
    return m_cursor_type_to_texture_id_map[static_cast<CursorType>(edit_mode)];
}

void Cursor::SetCursor(CursorType cursor_type) {
    m_cursor_type = cursor_type;
}

void Cursor::SetCursor(int edit_mode) {
    m_cursor_type = m_edit_mode_to_cursor_type_map[edit_mode];
}
