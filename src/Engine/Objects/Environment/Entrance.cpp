#include "Entrance.h"
#include "Engine/Application/Application.h"
#include "Engine/State/EntranceState.h"

Entrance::Entrance(Properties& props, std::string next_room_id)
    : Collider(props), m_next_room_id(next_room_id), m_closed(true) {}

Entrance::Entrance(Collider* collider, std::string next_room_id)
    : Collider(collider), m_next_room_id(next_room_id), m_closed(true) {
    m_animation->SelectAnimation("open");
}

void Entrance::Draw() {
    GameObject::Draw();
}

void Entrance::Update(float dt) {
    if (Application::Get().GetEnemyCount() == 0) {
        m_closed = false;
    }
    if (!m_closed) {
        m_animation->Update();
    }
}

void Entrance::OnCollide(Collider* collidee) {
    if (collidee->GetObjectType() == ObjectType::Player && !m_closed) {
        PushNewEvent(EventType::RoomTransitionEvent, &m_next_room_id);
        SDL_Log("ENTRANCE %s COLLIDED WITH PLAYER", GetID().c_str());
    }
}

void Entrance::Clean() {}
