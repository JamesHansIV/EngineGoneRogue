#pragma once

#include "GameObject.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "functional"
#include "SDL2/SDL.h"

class Projectile: public GameObject{

    public:
        Projectile(Properties& props, int speed, float mass, float angle);
        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt, const std::vector<GameObject*>& colliders);
        bool IsMarkedForDeletion() {return m_MarkedForDeletion;};
    private:
        Animation* m_Animation;
        RigidBody* m_RigidBody;
        int m_Speed;
        float m_Mass;
        float m_Angle;
        bool m_MarkedForDeletion;
        void CanMoveThrough(const std::vector<GameObject*>& colliders);
};
