#pragma once

#include "Engine/Objects/GameObject.h"
#include "Engine/Physics/CollisionBox.h"
#include "Engine/Physics/RigidBody.h"

// Reason: Cannot call OnCollide method in collider handler with GameObjects

class Collider : public GameObject {
   public:
    Collider(Properties& props, bool immovable = false)
        : GameObject(props), m_Immovable(immovable) {
        m_RigidBody = new RigidBody(GetX(), GetY());
        m_CollisionBox.Set(GetX(), GetY(), GetWidth(), GetHeight());
    }

    Collider(Collider* rhs);
    Collider(GameObject* rhs);

    void UnCollide(Collider* collidee);

    virtual void OnCollide(Collider* collider) {}

    RigidBody* GetRigidBody() { return m_RigidBody; }

    CollisionBox& GetCollisionBox() { return m_CollisionBox; }

    bool IsImmovable() { return m_Immovable; }

    virtual ObjectType GetObjectType() override { return ObjectType::Collider; }

   protected:
    RigidBody* m_RigidBody;
    CollisionBox m_CollisionBox;
    bool m_Immovable;
};