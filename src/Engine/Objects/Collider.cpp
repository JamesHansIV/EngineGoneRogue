#include "Collider.h"
#include "ColliderHandler.h"

Collider::Collider(Collider* rhs) : GameObject(rhs) {
    //needs testing
    m_RigidBody = new RigidBody(GetX(), GetY());

    m_Immovable = rhs->IsImmovable();
    SDL_Rect const rect = rhs->m_CollisionBox.GetRect();
    m_CollisionBox.Set(rect.x, rect.y, rect.w, rect.y);
}

Collider::Collider(GameObject* rhs) : GameObject(rhs) {
    //needs testing
    m_RigidBody = new RigidBody(GetX(), GetY());

    m_CollisionBox.Set(GetX(), GetY(), GetWidth(), GetHeight());
}

void Collider::UnCollide(Collider* collidee) {
    while (ColliderHandler::CheckCollision(
        m_CollisionBox.GetRect(), collidee->GetCollisionBox().GetRect())) {
        Vector2D const opposite_direction =
            m_RigidBody->Position() - collidee->GetRigidBody()->Position();
        m_RigidBody->MovePosition(opposite_direction * 0.01);
        SetX(m_RigidBody->Position().X);
        SetY(m_RigidBody->Position().Y);
        m_CollisionBox.Set(GetX(), GetY(), GetWidth(), GetHeight());
    }
}