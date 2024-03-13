#pragma once

#include "Engine/Objects/GameObject.h"
#include "Engine/Physics/CollisionBox.h"
#include "Engine/Physics/RigidBody.h"

// Reason: Cannot call OnCollide method in collider handler with GameObjects

class Collider : public GameObject {
   public:
    explicit Collider(Properties& props, bool immovable = false)
        : GameObject(props), m_immovable(immovable), m_do_un_collide(false) {
        m_rigid_body = new RigidBody(GetX(), GetY());
        m_collision_box.Set(GetX(), GetY(), GetWidth(), GetHeight());
    }

    explicit Collider(Collider* rhs);
    explicit Collider(GameObject* rhs);

    void SetDoUnCollide(bool doUnCollide) { m_do_un_collide = doUnCollide; }

    [[nodiscard]] bool GetDoUnCollide() const { return m_do_un_collide; }

    void UnCollide(Collider* collidee);

    virtual void OnCollide(Collider* collider) {}

    RigidBody* GetRigidBody() { return m_rigid_body; }

    CollisionBox& GetCollisionBox() { return m_collision_box; }

    [[nodiscard]] bool IsImmovable() const { return m_immovable; }

    ObjectType GetObjectType() override { return ObjectType::Collider; }

   protected:
    RigidBody* m_rigid_body;
    CollisionBox m_collision_box;
    bool m_immovable;
    //This indicates if a collider should call UnCollide in the OnCollide method
    bool m_do_un_collide;
};
