#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "functional"

class Player: public Character, public EventListener{

    public:
        Player(Properties& props);
        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt, const std::vector<GameObject*>& colliders);
        void OnEvent(Event& event);
        RigidBody* getRigidBody() {return m_RigidBody;};

    private:
        Animation* m_Animation;
        RigidBody* m_RigidBody;
        bool canMoveThrough(const std::vector<GameObject*>& colliders);
};

