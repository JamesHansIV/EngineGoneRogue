#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Physics/RigidBody.h"
#include "GameObject.h"
#include "SDL2/SDL.h"
#include "functional"

class Projectile : public Collider {

   public:
    Projectile(Properties& props, float speed, float angle,
               bool playerOwned = false);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    bool IsMarkedForDeletion() { return m_MarkedForDeletion; }

    void CheckOutOfBounds();

    virtual void OnCollide(Collider* collidee) override;

    bool PlayerOwned() { return m_PlayerOwned; }

    virtual ObjectType GetObjectType() override {
        return ObjectType::Projectile;
    }

   private:
    bool m_PlayerOwned;
    float m_Speed;
    float m_Angle;
    Vector2D m_Velocity;
    bool m_MarkedForDeletion;
};
