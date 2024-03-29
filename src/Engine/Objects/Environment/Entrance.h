#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"

class Entrance : public Collider {
   public:
    explicit Entrance(Properties& props, std::string next_room_id);
    Entrance(Collider* collider, std::string next_room_id);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    bool Closed() { return m_closed; }

    void Open() { m_closed = false; }

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Entrance; }

   private:
    bool m_closed;
    std::string m_next_room_id;
};
