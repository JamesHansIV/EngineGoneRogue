#include "Cursor.h"
#include "imgui.h"
#include "Engine/Application/Application.h"
#include<iostream>

Cursor::Cursor() {
    m_Width = 24;
    m_Height = 24;
    m_Scale = 1;
    m_Cursor_Type = CursorType::POINT;
}

Cursor::~Cursor() = default;

SDL_Rect Cursor::UpdateAndGetRect() {
    ImGuiIO const& io = ImGui::GetIO();  

    m_Cursor_Rect.h = m_Height * m_Scale;
    m_Cursor_Rect.w = m_Width * m_Scale;
    m_Cursor_Rect.x = io.MousePos.x;
    m_Cursor_Rect.y = io.MousePos.y - m_Cursor_Rect.h;

    return m_Cursor_Rect;
}

SDL_Rect Cursor::UpdateAndGetRect(int offsetX, int offsetY) {
    ImGuiIO const& io = ImGui::GetIO();  

    m_Cursor_Rect.h = m_Height * m_Scale;
    m_Cursor_Rect.w = m_Width * m_Scale;
    m_Cursor_Rect.x = io.MousePos.x + offsetX;
    m_Cursor_Rect.y = io.MousePos.y - m_Cursor_Rect.h + offsetY;

    return m_Cursor_Rect;
}

std::string Cursor::GetTextureId(int  /*edit_mode*/) {
    return cursor_type_to_texture_id_map[m_Cursor_Type];
}

void Cursor::SetCursor(CursorType cursor_type) {
    m_Cursor_Type = cursor_type;
    // m_Y_Offset = ;
}

void Cursor::SetCursor(EditMode edit_mode) {
    m_Cursor_Type = edit_mode_to_cursor_type_map[edit_mode];
}