#include "Cursor.h"
#include "imgui.h"
#include<iostream>

Cursor::Cursor() {
    m_Width = 18;
    m_Height = 18;
    m_Cursor_Type = CursorType::POINT;
    try {

    // check nullptr
    

    std::cout << "m_animation ptr" << m_Animation << std::endl;
    m_Animation->AddAnimation("draw_cursor", {"cursor_draw_texture", {0,0,m_Width,m_Height}, 0, 0});
    // m_Animation->AddAnimation("Dead", {"m_TextureID", {0, 0, 18, 16}, 6, 5}));
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

Cursor::~Cursor() {
    delete m_Animation;
}

void Cursor::Draw() {

    std::cout << "Trying to draw" << std::endl;

    // add logic for point
    if (m_Cursor_Type == CursorType::POINT)
        return;

    ImGuiIO& io = ImGui::GetIO();  
    Rect dstRect { io.MousePos.x, io.MousePos.y, m_Width, m_Height };
    m_Animation->Draw(dstRect, 0.0f);

}

void Cursor::SetCursor(CursorType cursor_type) {
    m_Cursor_Type = cursor_type;
}

void Cursor::SetCursor(int edit_mode) {
    m_Cursor_Type = edit_mode_to_cursor_type_map[edit_mode];
}