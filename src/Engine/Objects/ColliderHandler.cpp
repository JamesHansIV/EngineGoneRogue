#include "ColliderHandler.h"

ColliderHandler* ColliderHandler::s_instance = nullptr;

ColliderHandler::ColliderHandler()= default;

bool ColliderHandler::CheckCollision(SDL_Rect a, SDL_Rect b){
    if (a.x + a.w < b.x) {
        return false;
}
    if (b.x + b.w < a.x) {
        return false;
}

    if (a.y + a.h < b.y) {
        return false;
}
    if (b.y + b.h < a.y) {
        return false;
}

    return true;
}

void ColliderHandler::HandleCollisions() {
    for (auto it1 = m_Colliders.begin(); it1 != m_Colliders.end(); it1++) {
        for (auto it2 = m_Colliders.begin(); it2 != m_Colliders.end(); it2++) {
            Collider* c1 = *it1;
            Collider* c2 = *it2;
            if (CheckCollision(c1->GetCollisionBox().GetRect(), c2->GetCollisionBox().GetRect())) {
                c1->OnCollide(c2);
                c2->OnCollide(c1);
            }
        }
    }
}

void ColliderHandler::AddCollider(Collider* collider) {
    m_Colliders.push_back(collider);
}

void ColliderHandler::RemoveCollider(Collider* collider) {
    auto it = std::find(m_Colliders.begin(), m_Colliders.end(), collider);
    m_Colliders.erase(it);
}