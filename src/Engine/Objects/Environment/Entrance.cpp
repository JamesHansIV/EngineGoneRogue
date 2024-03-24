#include "Entrance.h"
#include "Engine/State/EntranceState.h"

Entrance::Entrance(Properties& props) : Collider(props) {
    m_animation = new Animation();
    m_animation->AddAnimation("open", {"door-open", GetTilePos(), 5, 15});
    m_animation->AddAnimation("close", {"door-close", GetTilePos(), 5, 15});
    ChangeState(new EntranceClosed(this));
}

void Entrance::Draw() {
    m_current_state->Draw();
}

void Entrance::Update(float dt) {
    State* state = m_current_state->Update(dt);

    if (state != nullptr) {
        ChangeState(state);
    }
}

void Entrance::OnCollide(Collider* collidee) {
    CollideEvent event(collidee);

    State* state = m_current_state->HandleEvent(&event);

    if (state != nullptr) {
        ChangeState(state);
    }
    }

void Entrance::Clean() {}
