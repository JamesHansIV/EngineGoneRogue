#pragma once

#include "Character.h"
#include "Engine/Objects/Collider.h"

class Enemy : public Character {
   public:
    explicit Enemy(Properties& props, const EnemyStats& stats);
    virtual void Draw() override = 0;
    virtual void Clean() override = 0;
    virtual void Update(float dt) override = 0;

    bool MoveTowardsTarget(float dt);

    bool TargetDetected();

    bool TargetInRange();

    Rect& GetPerception() { return m_Perception; }

    void SetTarget(Collider* target) { m_Target = target; }

    inline Collider* GetTarget() { return m_Target; }

    [[nodiscard]] EnemyStats GetEnemyStats() const { return m_stats; }

    void setEnemyStats(const EnemyStats& stats) { m_stats = stats; }

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    EnemyStats m_stats;
    Rect m_Perception;
    Collider* m_Target;
};
