#include "Entrance.h"
#include "Engine/Application/Application.h"
#include "Engine/State/EntranceState.h"

Entrance::Entrance(Properties& props, std::string curr_room_id,
                   std::string next_room_id, int x, int y)
    : Collider(props),
      m_curr_room_id(curr_room_id),
      m_next_room_id(next_room_id),
      m_next_start({x, y}),
      m_closed(true) {
    m_animation->SelectAnimation("open");
}

Entrance::Entrance(Collider* collider, std::string curr_room_id,
                   std::string next_room_id, int x, int y)
    : Collider(collider),
      m_curr_room_id(curr_room_id),
      m_next_room_id(next_room_id),
      m_next_start({x, y}),
      m_closed(true) {
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
        Application::Get().AddRoomCleared(m_curr_room_id);
        Application::Get().SetStartPosition(m_next_start.first,
                                            m_next_start.second);
        if (m_next_room_id == "") {
            SDL_Log("entrance in %s has empty next room id",
                    m_curr_room_id.c_str());
            assert(false);
        }
        PushNewEvent(EventType::RoomTransitionEvent, &m_next_room_id);
        SDL_Log("ENTRANCE %s COLLIDED WITH PLAYER", GetID().c_str());
    }
}

void Entrance::Clean() {}
