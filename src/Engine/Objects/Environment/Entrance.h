#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"

class Entrance : public Collider {
   public:
    explicit Entrance(Properties& props, std::string curr_room_id,
                      std::string next_room_id, int x, int y);
    Entrance(Collider* collider, std::string curr_room_id,
             std::string next_room_id, int x, int y);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    bool Closed() { return m_closed; }

    void Open() { m_closed = false; }

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Entrance; }

   private:
    bool m_closed;
    std::string m_curr_room_id;
    std::string m_next_room_id;
    std::pair<int, int> m_next_start;
};
