#include "Collider.h"
#include "ColliderHandler.h"

Collider::Collider(Collider* rhs) : GameObject(rhs) {
    //needs testing
    m_rigid_body = new RigidBody(GetX(), GetY());

    m_immovable = rhs->IsImmovable();
    Rect const rect = rhs->m_collision_box.GetRect();
    m_collision_box.Set(rect.x, rect.y, rect.w, rect.y);
}

Collider::Collider(GameObject* rhs) : GameObject(rhs) {
    //needs testing
    m_rigid_body = new RigidBody(GetX(), GetY());

    m_collision_box.Set(GetX(), GetY(), GetWidth(), GetHeight());
}

Collider::~Collider() {
    if (m_rigid_body != nullptr) {
        delete m_rigid_body;
        m_rigid_body = nullptr;
    }
}

void Collider::UnCollide(Collider* collidee) {
    if (!m_do_un_collide) {
        return;
    }

    Vector2D velocity = GetRigidBody()->Velocity();
    Vector2D const position_diff =
        collidee->GetRigidBody()->Position() - m_rigid_body->Position();

    if (position_diff * velocity <= 0) {
        return;
    }
    //ColliderHandler::GetInstance()->MoveToEdge(this, collidee);
    Rect c_rect1 = {GetRigidBody()->Position().X + velocity.X,
                    GetRigidBody()->Position().Y + velocity.Y, GetWidth(),
                    GetHeight()};
    Rect const c_rect2 = {collidee->GetRigidBody()->Position().X,
                          collidee->GetRigidBody()->Position().Y,
                          collidee->GetWidth(), collidee->GetHeight()};
    // SDL_Log("collider rect: (%f, %f)", cRect1.x, cRect1.y);
    // SDL_Log("collidee rect: (%f, %f)", cRect2.x, cRect2.y);
    // SDL_Log("previous velocity: ");
    // velocity.Log();

    while (ColliderHandler::CheckCollision(c_rect1, c_rect2)) {
        velocity = velocity - (position_diff * 0.01);
        c_rect1.x = GetRigidBody()->Position().X + velocity.X;
        c_rect1.y = GetRigidBody()->Position().Y + velocity.Y;
        // SDL_Log("new velocity: (%f, %f)", velocity.X, velocity.Y);
        // SDL_Log("new collider rect: (%f, %f)", cRect1.x, cRect1.y);
    }
    // SDL_Log("updated velocity: ");
    // velocity.Log();

    GetRigidBody()->SetVelocity(velocity);

    m_do_un_collide = false;
}
