#pragma once

#include "Enemy.h"

class Slime : public Enemy {
   public:
    explicit Slime(Properties& props, int perceptionWidth, int perceptionHeight,
                   float range = 45.0F, bool split = false);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void OnCollide(Collider* collidee) override;

    bool IsSplit() { return m_Split; }

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    bool m_Split;
};
