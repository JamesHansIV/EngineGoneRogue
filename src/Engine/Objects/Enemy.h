#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "functional"

class Enemy: public Character, public EventListener{
    public:
        explicit Enemy(Properties& props);
        void Draw() override;
        void Clean() override;
        void Update(float dt) override;
        void OnEvent(Event& event) override;
        RigidBody* GetRigidBody() {return m_RigidBody;};

    private:
        Animation* m_Animation;
        RigidBody* m_RigidBody;
        void CanMoveThrough(const std::vector<GameObject*>& colliders);
        void DrawEnemyHealth();
};

