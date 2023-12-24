#include "GameObject.h"

GameObject::GameObject(GameObject* rhs) {
    m_TextureID = rhs->m_TextureID;
    m_CurrentTilePos = rhs->m_CurrentTilePos;
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
}

void GameObject::Draw() {
    if (m_Animation != nullptr) {
        DrawAnimation();
    } else {
        DrawRect();
    }
};

void GameObject::DrawRect() {
    SDL_Rect src_rect = {m_CurrentTilePos.col * m_CurrentTilePos.w,
                         m_CurrentTilePos.row * m_CurrentTilePos.h,
                         m_CurrentTilePos.w, m_CurrentTilePos.h};
    SDL_Rect dst_rect = {static_cast<int>(m_DstRect.x),
                         static_cast<int>(m_DstRect.y), m_DstRect.w,
                         m_DstRect.h};
    Renderer::GetInstance()->Draw(m_TextureID, src_rect, dst_rect, m_Rotation,
                                  nullptr, m_Flip);
}

void GameObject::DrawAnimation() {
    m_Animation->Draw(m_DstRect, m_Rotation);
}

void GameObject::Update(float /*dt*/) {
    if (m_Animation != nullptr) {
        m_Animation->Update();
    }
}

void GameObject::AddStillFrame(const std::string& id, TilePos tilePos) {
    if (m_StillFrames.find(id) != m_StillFrames.end()) {
        SDL_Log("TilePos with id %s already exists for object %s", id.c_str(),
                GetID().c_str());
        assert(false);
    }
    m_StillFrames[id] = tilePos;
}

void GameObject::SelectStillFrame(const std::string& id) {
    if (m_StillFrames.find(id) == m_StillFrames.end()) {
        SDL_Log("TilePos with id=%s does not exist for object %s", id.c_str(),
                GetID().c_str());
        assert(false);
    }
    m_CurrentTilePos = m_StillFrames[id];
}

void GameObject::ChangeState(State* state) {
    if (m_CurrentState != nullptr) {
        m_CurrentState->Exit();
        delete m_CurrentState;
    }

    m_CurrentState = state;
    m_CurrentState->Enter();
}
