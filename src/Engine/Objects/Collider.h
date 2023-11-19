#pragma once

#include "Engine/Physics/CollisionBox.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Objects/GameObject.h"

class Collider : public GameObject{
    public:
        Collider(Properties& props) : GameObject(props) {
            m_CollisionBox.Set(GetX(), GetY(), GetWidth(), GetHeight());
        }

        Collider(Collider* rhs);

        virtual void OnCollide(Collider* collider);

        RigidBody& GetRigidBody() { return m_RigidBody; }
        CollisionBox& GetCollisionBox() { return m_CollisionBox; }

        virtual ObjectType GetObjectType() override { return ObjectType::kCollider; }
    protected:
        RigidBody m_RigidBody;
        CollisionBox m_CollisionBox;
};