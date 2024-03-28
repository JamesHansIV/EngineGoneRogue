#pragma once

#include "Character.h"
#include "Engine/Objects/Collider.h"

class EnemyState;

class Enemy : public Character {
   public:
    explicit Enemy(Properties& props, const EnemyStats& stats);
    Enemy(Collider* rhs, EnemyStats stats);

    void Draw() override { GameObject::Draw(); }

    void Clean() override = 0;
    void Update(float dt) override = 0;

    bool MoveTowardsTarget(float dt);

    bool TargetDetected();

    bool TargetInRange();

    virtual void Attack() {}

    Rect& GetPerception() { return m_perception; }

    void SetTarget(Collider* target) { m_target = target; }

    inline Collider* GetTarget() { return m_target; }

    [[nodiscard]] EnemyStats GetEnemyStats() const { return m_stats; }

    EnemyStats& GetMutableEnemyStats() { return m_stats; }

    void SetEnemyStats(const EnemyStats& stats) { m_stats = stats; }

    virtual EnemyState* GetHitState(int damage);
    virtual EnemyState* GetIdleState();
    virtual EnemyState* GetMovingState();
    virtual EnemyState* GetAttackState();
    virtual EnemyState* GetDeadState();

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   protected:
    EnemyStats m_stats;
    Rect m_perception;
    Collider* m_target;
};
