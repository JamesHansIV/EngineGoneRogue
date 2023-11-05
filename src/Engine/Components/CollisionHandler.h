#pragma once

#include "SDL2/SDL.h"
#include <vector>

class CollisionHandler{

    public:
        bool CheckCollision(SDL_Rect a, SDL_Rect b);
        inline static CollisionHandler* GetInstance(){ return s_Instance = (s_Instance != nullptr)? s_Instance : new CollisionHandler();}
    private:
        CollisionHandler();
        static CollisionHandler* s_Instance;
};