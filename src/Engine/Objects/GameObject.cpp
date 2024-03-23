#include "GameObject.h"

GameObject::GameObject(GameObject* rhs) {
    m_current_state = nullptr;
    m_texture_id = rhs->m_texture_id;
    m_current_tile_pos = rhs->m_current_tile_pos;
    m_dst_rect = rhs->m_dst_rect;
    m_rotation = rhs->m_rotation;
    m_flip = rhs->m_flip;
    m_object_id = rhs->m_object_id;
    m_marked_for_deletion = false;

    if (rhs->m_animation != nullptr) {
        m_animation = new Animation(rhs->m_animation);
    }
}

GameObject::~GameObject() {
    if (m_animation != nullptr) {
        delete m_animation;
    }
    if (m_current_state != nullptr) {
        delete m_current_state;
    }
}

void GameObject::Draw() {
    if (m_animation != nullptr) {
        DrawAnimation();
    } else {
        DrawRect();
    }
};

void GameObject::DrawRect() {
    SDL_Rect src_rect = {m_current_tile_pos.col * m_current_tile_pos.w,
                         m_current_tile_pos.row * m_current_tile_pos.h,
                         m_current_tile_pos.w, m_current_tile_pos.h};
    SDL_Rect dst_rect = {static_cast<int>(m_dst_rect.x),
                         static_cast<int>(m_dst_rect.y), m_dst_rect.w,
                         m_dst_rect.h};
    Renderer::GetInstance()->Draw(m_texture_id, src_rect, dst_rect, m_rotation,
                                  nullptr, m_flip);
}

void GameObject::DrawAnimation() {
    m_animation->Draw(m_dst_rect, m_rotation);
}

void GameObject::Update(float /*dt*/) {
    if (m_animation != nullptr) {
        m_animation->Update();
    }
}

void GameObject::AddStillFrame(const std::string& id, TilePos tilePos) {
    if (m_still_frames.find(id) != m_still_frames.end()) {
        SDL_Log("TilePos with id %s already exists for object %s", id.c_str(),
                GetID().c_str());
        assert(false);
    }
    m_still_frames[id] = tilePos;
}

void GameObject::SelectStillFrame(const std::string& id) {
    if (m_still_frames.find(id) == m_still_frames.end()) {
        SDL_Log("TilePos with id=%s does not exist for object %s", id.c_str(),
                GetID().c_str());
        assert(false);
    }
    m_current_tile_pos = m_still_frames[id];
}

void GameObject::ChangeState(State* state) {
    if (m_current_state != nullptr) {
        m_current_state->Exit();
        delete m_current_state;
    }

    m_current_state = state;
    m_current_state->Enter();
}
