#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Objects/Collider.h"
#include "functional"

class Enemy: public Character, public EventListener{
    public:
        explicit Enemy(Properties& props, int perceptionX, int perceptionY);
        virtual void Draw() override;
        virtual void Clean() override;
        virtual void Update(float dt) override;
        virtual void OnEvent(Event& event) override;

        bool IsMarkedForDeletion(){return m_MarkedForDeletion;}
        void SetPlayer(Collider* pc) {m_Player = pc;}
        void UpdateColliders(const std::vector<Collider*>& colliders){m_Colliders = colliders;}

        virtual void OnCollide(Collider* collidee) override;
        virtual ObjectType GetObjectType() override { return ObjectType::kEnemy; }
    private:
        Animation* m_Animation;
        int m_PerceptionWidth;
        int m_PerceptionHeight;
        SDL_Rect m_Perception;
        bool m_MarkedForDeletion;
        void CanMoveThrough();
        void DrawEnemyHealth();
        Collider* m_Player;
        std::vector<Collider*> m_Colliders;
};

