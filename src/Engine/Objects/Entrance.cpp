#include "Entrance.h"

Entrance::Entrance(Properties& props) : Collider(props) {
    m_State.AddState(EntranceState::Idle);
    m_Animation = new Animation();
    m_Animation->AddAnimation("open", {"door-open", GetTilePos(), 5, 15});
    m_Animation->AddAnimation("close", {"door-close", GetTilePos(), 5, 15});
    //We have to add idle frames for the door to render
    m_Animation->SelectAnimation("open");
    m_Animation->StopAnimation();
}

void Entrance::Draw() {
    m_Animation->Draw(GetDstRect());
}

void Entrance::Update(float  /*dt*/) {
    if (!m_State.HasState(EntranceState::Idle) && m_Animation->Ended()) {
        m_State.ToggleState(EntranceState::Open);
        m_State.AddState(EntranceState::Idle);
    }
    m_Animation->Update();
}

void Entrance::OnCollide(Collider* collidee) {
    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            if (m_State.HasState(EntranceState::Idle)) {
                m_State.RemoveState(EntranceState::Idle);
                if (m_State.HasState(EntranceState::Open)) {
                    m_Animation->SelectAnimation("close");
                } else {
                    m_Animation->SelectAnimation("open");
                }
            } 
            break;

        case ObjectType::Enemy:
            break;
        default:
            break;
    }
}

void Entrance::Clean() {}