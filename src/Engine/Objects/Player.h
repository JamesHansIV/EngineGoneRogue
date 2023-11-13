#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "functional"

class Player: public Character, public EventListener{

    public:
        explicit Player(Properties& props);
        void Draw() override;
        void Clean() override;
        void Update(float dt) override;
        void UpdateColliders(const std::vector<GameObject*>& colliders){m_Colliders = colliders;};
        void OnEvent(Event& event) override;
        RigidBody* GetRigidBody() {return m_RigidBody;};

    private:
        Animation* m_Animation;
        RigidBody* m_RigidBody;
        void CanMoveThrough();
        void DrawPlayerHealth();
        std::vector<GameObject*> m_Colliders;
};

