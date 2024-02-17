#pragma once

#include "Enemy.h"

class Slime : public Enemy {
   public:
    explicit Slime(Properties& props, const EnemyStats& stats,
                   bool split = false);
    Slime(Collider& rhs, EnemyStats stats);
    void Init();

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void OnCollide(Collider* collidee) override;

    bool IsSplit() { return m_Split; }

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    bool m_Split;
};
