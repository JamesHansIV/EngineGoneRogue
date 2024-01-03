#pragma once

#include "Engine/Objects/RangedEnemy.h"

struct GoblinStats {
    float Spread;
    int BurstDuration;
    int BurstCD;
    void Init();
    void SetBurstDuration();
    void SetBurstCD();
};

class Goblin : public RangedEnemy {
   public:
    explicit Goblin(Properties& props, int perceptionWidth,
                    int perceptionHeight, float range = 150.0f,
                    int fireInterval = 7);

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() override;

    virtual void OnCollide(Collider* collidee) override;

    GoblinStats& GetStats() { return m_Stats; }

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    GoblinStats m_Stats;
};
