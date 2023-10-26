#pragma once

#include "Character.h"
#include "../Components/Animation.h"
#include "../Components/RigidBody.h"

class Warrior: public Character{

    public:
        Warrior(Properties& props);

        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);
        
        virtual void OnEvent(Event& e);

    private:
        Animation* m_Animation;
        RigidBody* m_RigidBody; 
};

