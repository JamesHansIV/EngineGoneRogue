#pragma once

#include "GameObject.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Objects/Collider.h"
#include "functional"
#include "SDL2/SDL.h"

class Projectile: public Collider{

    public:
        Projectile(Properties& props, float speed, float angle, bool playerOwned = false);
        virtual void Draw() override;
        virtual void Clean() override;
        virtual void Update(float dt) override;
        bool IsMarkedForDeletion() {return m_MarkedForDeletion;}
        void CheckOutOfBounds();

        virtual void OnCollide(Collider* collidee) override;

        virtual ObjectType GetObjectType() override { return ObjectType::kProjectile; }
        virtual ObjectCategory GetObjectCategory() override { return ObjectCategory::Projectile; }
    private:
        bool m_PlayerOwned;
        float m_Speed;
        float m_Angle;
        Vector2D m_Velocity;
        bool m_MarkedForDeletion;
};
