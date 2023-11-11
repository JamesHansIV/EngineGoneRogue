#pragma once

#include "SDL2/SDL.h"
#include <vector>

class CollisionHandler{

    public:
        static bool CheckCollision(SDL_Rect a, SDL_Rect b);
        inline static CollisionHandler* GetInstance(){ return s_instance = (s_instance != nullptr)? s_instance : new CollisionHandler();}
    private:
        CollisionHandler();
        static CollisionHandler* s_instance;
};