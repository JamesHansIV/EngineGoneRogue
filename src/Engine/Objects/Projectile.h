#pragma once

#include "GameObject.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Objects/Collider.h"
#include "functional"
#include "SDL2/SDL.h"

class Projectile: public Collider{

    public:
        Projectile(Properties& props, int speed, float mass, float angle);
        virtual void Draw() override;
        virtual void Clean() override;
        virtual void Update(float dt, const std::vector<Collider*>& colliders);
        bool IsMarkedForDeletion() {return m_MarkedForDeletion;}
        virtual ObjectType GetObjectType() override { return ObjectType::kProjectile; }
    private:
        Animation* m_Animation;
        int m_Speed;
        float m_Angle;
        bool m_MarkedForDeletion;
        void CanMoveThrough(const std::vector<Collider*>& colliders);
};
