#include "Entrance.h"
#include "Engine/State/EntranceState.h"

Entrance::Entrance(Properties& props) : Collider(props) {
    m_Animation = new Animation();
    m_Animation->AddAnimation("open", {"door-open", GetTilePos(), 5, 15});
    m_Animation->AddAnimation("close", {"door-close", GetTilePos(), 5, 15});
    ChangeState(new EntranceClosed(this));
}

void Entrance::Draw() {
    m_CurrentState->Draw();
}

void Entrance::Update(float dt) {
    State* state = m_CurrentState->Update(dt);

    if (state != nullptr) {
        ChangeState(state);
    }
}

void Entrance::OnCollide(Collider* collidee) {
    CollideEvent event(collidee);

    State* state = m_CurrentState->HandleEvent(&event);

    if (state != nullptr) {
        ChangeState(state);
    }
    return;
}

void Entrance::Clean() {}
