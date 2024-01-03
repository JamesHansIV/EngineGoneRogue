#pragma once
#include "RangedEnemy.h"

class Skeleton : public RangedEnemy {
   public:
    explicit Skeleton(Properties& props, int perceptionWidth,
                      int perceptionHeight, float range = 150.0F,
                      int fireInterval = 50, float spread = 60.0F,
                      int spreadCount = 4);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    float m_Spread;
    int m_SpreadCount;
};
