#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Physics/RigidBody.h"
#include "functional"

class Enemy : public Character {
   public:
    explicit Enemy(Properties& props, int perceptionWidth, int perceptionHeight, float range);
    virtual void Draw() override = 0;
    virtual void Clean() override = 0;
    virtual void Update(float dt) override = 0;

    bool MoveTowardsTarget(float dt, float range);

    bool TargetDetected();

    float GetRange() { return m_Range; }
    
    Rect& GetPerception() { return m_Perception; }

    void SetTarget(Collider* target) { m_Target = target; }

    inline Collider* GetTarget() { return m_Target; }

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    int m_PerceptionWidth;
    int m_PerceptionHeight;
    float m_Range;
    Rect m_Perception;
    Collider* m_Target;
};
