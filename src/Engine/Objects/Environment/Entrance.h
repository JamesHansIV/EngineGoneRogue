#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"

class Entrance : public Collider {
   public:
    explicit Entrance(Properties& props);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Entrance; }
};
