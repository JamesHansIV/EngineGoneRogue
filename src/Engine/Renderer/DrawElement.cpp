#include "DrawElement.h"
#include "Engine/Renderer/Renderer.h"

void DrawElement::Draw(TilePos src) {
    SDL_Rect src_rect = {src.col * src.w, src.row * src.h, src.w, src.h};
    SDL_Rect dst_rect = {m_Dst.x, m_Dst.y, m_Dst.w, m_Dst.h};

    //Renderer::GetInstance()->DrawRelative(m_TextureID, src_rect, dst_rect);
}
