#include "Collider.h"

Collider::Collider(Collider* rhs) : GameObject(rhs) {
    m_RigidBody = rhs->m_RigidBody;

    SDL_Rect const rect = rhs->m_CollisionBox.GetRect();
    m_CollisionBox.Set(
        rect.x,
        rect.y,
        rect.w,
        rect.y
    );
}

void Collider::OnCollide(Collider* collidee) {

}