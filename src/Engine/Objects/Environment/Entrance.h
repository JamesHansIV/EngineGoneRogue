#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"

const int kDefaultCollidelessTime = 50;

class Entrance : public Collider {
   public:
    explicit Entrance(Properties& props, std::string curr_room_id,
                      std::string next_room_id, int x, int y,
                      int collidless_time = kDefaultCollidelessTime);
    Entrance(Collider* collider, std::string curr_room_id,
             std::string next_room_id, int x, int y,
             int collideless_time = kDefaultCollidelessTime);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    GameObject* Copy() override;

    std::string GetCurrentRoomID() { return m_curr_room_id; }

    std::string GetNextRoomID() { return m_next_room_id; }

    std::pair<int, int> GetNextStart() { return m_next_start; }

    bool Closed() { return m_closed; }

    void Open() { m_closed = false; }

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Entrance; }

   private:
    int m_collideless_time;
    bool m_closed;
    std::string m_curr_room_id;
    std::string m_next_room_id;
    std::pair<int, int> m_next_start;
};
