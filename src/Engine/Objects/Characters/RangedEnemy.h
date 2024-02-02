#pragma once

#include "Enemy.h"
#include "Engine/Objects/Attacks/AttackFunctions.h"

class RangedEnemy : public Enemy {
   public:
    explicit RangedEnemy(Properties& props, int perceptionWidth,
                         int perceptionHeight, float range, int fireInterval)
        : Enemy(props, perceptionWidth, perceptionHeight, range),
          m_FireInterval(fireInterval) {
        m_Animation = new Animation();
    }

    ~RangedEnemy();

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void Shoot() = 0;

    int GetFireInterval() { return m_FireInterval; }

    RangedAttack* GetAttack() { return m_Attack; }

    void SetAttack(RangedAttack* attack) { m_Attack = attack; }

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    RangedAttack* m_Attack;
    int m_FireInterval;
};
