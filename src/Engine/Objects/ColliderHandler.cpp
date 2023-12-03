#include "ColliderHandler.h"

ColliderHandler* ColliderHandler::s_instance = nullptr;

ColliderHandler::ColliderHandler() = default;

bool ColliderHandler::CheckCollision(Rect a, Rect b) {
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



float ColliderHandler::FindIntersection(Vector2D p1, Vector2D d1, Vector2D p2, Vector2D d2) {
    Vector2D b { p2.X - p1.X, p2.Y - p1.Y };
    Matrix2D A { d1.X, -d2.X, d1.Y, -d2.Y };

    if (A.Determinant() == 0.0f) {
        return 0.0f;
    }

    Vector2D t = A.Inverse() * b;

    t.Log();
    return t.X;
}

void ColliderHandler::MoveToEdge(Collider* c1, Collider* c2) {
    //Get collider rect
    Rect cRect1 = {
        c1->GetRigidBody()->Position().X + c1->GetRigidBody()->Velocity().X,
        c1->GetRigidBody()->Position().Y + c1->GetRigidBody()->Velocity().Y,
        c1->GetWidth(),
        c1->GetHeight()
    };
    //Get collidee rect
    Rect cRect2 = {
        c2->GetRigidBody()->Position().X,
        c2->GetRigidBody()->Position().Y,
        c2->GetWidth(),
        c2->GetHeight()
    };
    
    //Define intersecting vector
    Vector2D pos {
        static_cast<float>(cRect1.x + 0.5 * cRect1.w),
        static_cast<float>(cRect1.y + 0.5 * cRect1.h)
    };
    Vector2D direction = c1->GetRigidBody()->Velocity();
    
    //This should not be greater than 1
    float minT = 1.0f;

    //Define minkowski sum of collidee
    Vector2D minPos {
        static_cast<float>(cRect2.x - 0.5 * cRect1.w),
        static_cast<float>(cRect2.y - 0.5 * cRect1.h)
    };

    Vector2D maxPos {
        minPos.X + cRect1.w + cRect2.w,
        minPos.Y + cRect1.h + cRect2.h
    };

    Vector2D positions[4] = {
        minPos,
        maxPos,
        maxPos,
        minPos
    };
    Vector2D directions[4] = {
        Vector2D(0, 1),
        Vector2D(0, -1),
        Vector2D(-1, 0),
        Vector2D(1, 0)
    };
    SDL_Log("collider rect: (%f, %f)", cRect1.x, cRect1.y);
    SDL_Log("collidee rect: (%f, %f)", cRect2.x, cRect2.y);
    SDL_Log("p1: (%f, %f)", pos.X, pos.Y);
    SDL_Log("d1: (%f, %f)", direction.X, direction.Y);
    for (int i = 0; i < 4; i++) {
            
        SDL_Log("p2: (%f, %f)", positions[i].X, positions[i].Y);
        SDL_Log("d2: (%f, %f)", directions[i].X, directions[i].Y);
        const float t = FindIntersection(pos, direction, positions[i], directions[i]);
        if (t <= 0.0f) continue;
        minT = t < minT ? t : minT;
    }

    assert(minT > 0);

    std::cout << "Prev move: ";
    c1->GetRigidBody()->Velocity().Log();

    Vector2D nextMove = direction * minT;
    std::cout << "Next move: ";
    nextMove.Log();

    c1->GetRigidBody()->SetVelocity(direction * minT);
    assert(false);
}

void ColliderHandler::HandleCollisions() {
    for (int i = 0; i < m_Colliders.size(); i++) {
        for (int j = i + 1; j < m_Colliders.size(); j++) {
            Collider* c1 = m_Colliders[i];
            Collider* c2 = m_Colliders[j];

            RigidBody* r1 = c1->GetRigidBody();
            RigidBody* r2 = c2->GetRigidBody();


            Rect nextPos1 = {
                r1->Position().X + r1->Velocity().X,
                r1->Position().Y + r1->Velocity().Y,
                c1->GetWidth(),
                c1->GetHeight()
            };
            Rect nextPos2 = {
                r2->Position().X + r2->Velocity().X,
                r2->Position().Y + r2->Velocity().Y,
                c2->GetWidth(),
                c2->GetHeight()
            };
            
            if (!r1->Velocity().IsZero() && CheckCollision(nextPos1, c2->GetDstRect())) {
                c1->OnCollide(c2);
            }
            if (!r2->Velocity().IsZero() && CheckCollision(nextPos2, c1->GetDstRect())) {
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