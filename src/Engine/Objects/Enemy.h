#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "functional"

class Enemy: public Character, public EventListener{
    public:
        explicit Enemy(Properties& props, int perceptionX, int perceptionY);
        void Draw() override;
        void Clean() override;
        void Update(float dt) override;
        void OnEvent(Event& event) override;
        RigidBody* GetRigidBody() {return m_RigidBody;};
        bool IsMarkedForDeletion(){return m_MarkedForDeletion;};
        void SetPlayer(GameObject* pc) {m_Player = pc;};
        void UpdateColliders(const std::vector<GameObject*>& colliders){m_Colliders = colliders;};
    private:
        Animation* m_Animation;
        RigidBody* m_RigidBody;
        int m_PerceptionX;
        int m_PerceptionY;
        SDL_Rect m_Perception;
        bool m_MarkedForDeletion;
        void CanMoveThrough();
        void DrawEnemyHealth();
        GameObject* m_Player;
        std::vector<GameObject*> m_Colliders;
};

