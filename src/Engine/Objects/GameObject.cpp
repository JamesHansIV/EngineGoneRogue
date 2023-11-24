#include "GameObject.h"

GameObject::GameObject(GameObject* rhs) {
    m_TextureID = rhs->m_TextureID;
    m_TilePos = rhs->m_TilePos;
    m_DstRect = rhs->m_DstRect;
    m_Rotation = rhs->m_Rotation;
    m_Flip = rhs->m_Flip;
    m_ObjectID = rhs->m_ObjectID;

    if (rhs->m_Animation != nullptr) {
        m_Animation = new Animation();
        m_Animation->SetProps({rhs->m_Animation->GetTextureID(),
                               {rhs->m_Animation->GetSpriteRow(),
                                rhs->m_Animation->GetSpriteCol(),
                                rhs->m_Animation->GetSpriteWidth(),
                                rhs->m_Animation->GetSpriteHeight()},
                               rhs->m_Animation->GetFrameCount(),
                               rhs->m_Animation->GetAnimationSpeed()});
    }

    m_Transform = new Transform(&m_DstRect.x, &m_DstRect.y);
}

void GameObject::Draw() {
    if (m_Animation != nullptr) {
        m_Animation->Draw(m_DstRect, m_Rotation);
    } else {
        SDL_Rect src_rect = {m_TilePos.col * m_TilePos.w,
                             m_TilePos.row * m_TilePos.h, m_TilePos.w,
                             m_TilePos.h};
        SDL_Rect dst_rect = {static_cast<int>(m_DstRect.x),
                             static_cast<int>(m_DstRect.y), m_DstRect.w,
                             m_DstRect.h};
        Renderer::GetInstance()->Draw(m_TextureID, src_rect, dst_rect,
                                      m_Rotation, nullptr, m_Flip);
    }
};

void GameObject::Update(float /*dt*/) {
    if (m_Animation != nullptr) {
        m_Animation->Update();
    }
}