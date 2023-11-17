#pragma once

#include "Collider.h"
#include "SDL2/SDL.h"
#include <vector>

class ColliderHandler{
    public:
        static bool CheckCollision(SDL_Rect a, SDL_Rect b);
        void AddCollider(Collider* collider);
        void RemoveCollider(Collider* collider);
        void CheckCollisions();
        inline static ColliderHandler* GetInstance(){ return s_instance = (s_instance != nullptr) ? s_instance : new ColliderHandler();}
    private:
        std::vector<Collider*> m_Colliders;
        ColliderHandler();
        static ColliderHandler* s_instance;
};