#include "Entrance.h"

Entrance::Entrance(Properties& props) : Collider(props) {
    m_State.AddState(EntranceState::Idle);
}

void Entrance::Draw() {
    if (m_State.HasState(EntranceState::Idle)) {
        GameObject::Draw();
    } else {
        m_Animation->Draw(GetDstRect());
    }
}

void Entrance::Update(float dt) {}

void Entrance::OnCollide(Collider* collidee) {

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            m_State.SetState(EntranceState::Open);
            m_Animation->SelectAnimation("Open");
            break;
        default:
            break;
    }
}

void Entrance::Clean() {}