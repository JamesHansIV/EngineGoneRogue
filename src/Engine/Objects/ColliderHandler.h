#pragma once

#include <vector>
#include "Collider.h"
#include "SDL2/SDL.h"

class ColliderHandler {
   public:
    static bool CheckCollision(Rect a, Rect b);
    static float FindIntersection(Vector2D p1, Vector2D d1, Vector2D p2,
                                  Vector2D d2);
    static void MoveToEdge(Collider* c1, Collider* c2);
    void AddCollider(Collider* collider);
    void RemoveCollider(Collider* collider);
    void HandleCollisions();

    inline static ColliderHandler* GetInstance() {
        return s_instance =
                   (s_instance != nullptr) ? s_instance : new ColliderHandler();
    }

   private:
    std::vector<Collider*> m_colliders;
    ColliderHandler();
    static ColliderHandler* s_instance;
};
