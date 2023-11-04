#pragma once

#include "Character.h"
#include "Engine/Components/Animation.h"
#include "Engine/Components/Physics/RigidBody.h"
#include "functional"

class Warrior: public Character, public EventListener{

    public:
        Warrior(Properties& props);

        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);
        void OnEvent(Event& event);
        RigidBody* getRigidBody() {return m_RigidBody;};
    private:
        Animation* m_Animation;
        RigidBody* m_RigidBody;
};

