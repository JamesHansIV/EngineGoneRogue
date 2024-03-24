#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"

class Entrance : public Collider {
   public:
    explicit Entrance(Properties& props);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Entrance; }
};
