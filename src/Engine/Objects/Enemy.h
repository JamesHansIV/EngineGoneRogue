#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Objects/Collider.h"
#include "functional"

class Enemy: public Character{
    public:
        explicit Enemy(Properties& props, int perceptionX, int perceptionY);
        virtual void Draw() override;
        virtual void Clean() override;
        virtual void Update(float dt) override;

        void MoveTowardsTarget(float dt);

        bool IsMarkedForDeletion(){return m_MarkedForDeletion;}
        void SetTarget(Collider* target) {m_Target = target;}

        virtual void OnCollide(Collider* collidee) override;
        virtual ObjectType GetObjectType() override { return ObjectType::kEnemy; }
    private:
        Animation* m_Animation;
        int m_PerceptionWidth;
        int m_PerceptionHeight;
        SDL_Rect m_Perception;
        bool m_MarkedForDeletion;
        Collider* m_Target;
};

