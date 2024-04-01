#include "Entrance.h"
#include "Engine/Application/Application.h"
#include "Engine/State/EntranceState.h"

Entrance::Entrance(Properties& props, std::string curr_room_id,
                   std::string next_room_id, int x, int y, int collideless_time)
    : Collider(props),
      m_curr_room_id(curr_room_id),
      m_next_room_id(next_room_id),
      m_next_start({x, y}),
      m_closed(true),
      m_collideless_time(collideless_time) {
    m_collision_box.SetCorrection(5, 5, 12, 12);
    m_animation->SelectAnimation("open");
}

Entrance::Entrance(Collider* collider, std::string curr_room_id,
                   std::string next_room_id, int x, int y, int collideless_time)
    : Collider(collider),
      m_curr_room_id(curr_room_id),
      m_next_room_id(next_room_id),
      m_next_start({x, y}),
      m_closed(true),
      m_collideless_time(collideless_time) {
    m_collision_box.SetCorrection(5, 0, 12, 12);
    m_animation->SelectAnimation("open");
}

void Entrance::Draw() {
    GameObject::Draw();
    //Rect box = m_collision_box.GetRect();
    //SDL_Rect rect = {(int)box.x, (int)box.y, box.w, box.h};
    //Renderer::GetInstance().DrawRect(rect, {255, 255, 255, 255});
}

void Entrance::Update(float dt) {
    if (m_collideless_time > 0) {
        m_collideless_time--;
    }

    if (Application::Get().GetEnemyCount() == 0) {
        m_closed = false;
    }
    if (!m_closed) {
        m_animation->Update();
    }
}

GameObject* Entrance::Copy() {
    return new Entrance(this, m_curr_room_id, m_next_room_id,
                        m_next_start.first, m_next_start.second);
}

void Entrance::OnCollide(Collider* collidee) {
    SDL_Log("collide with something");
    if (collidee->GetObjectType() == ObjectType::Player && !m_closed &&
        m_collideless_time == 0) {
        SDL_Log("player collide with entrance");
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
