#pragma once

#include "GameObject.h"
#include "Engine/Components/Animation.h"
#include "Engine/Components/Physics/RigidBody.h"
#include "functional"

class Projectile: public GameObject{

    public:
        Projectile(Properties& props);
        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);

    private:
        Animation* m_Animation;
        RigidBody* m_RigidBody;
};