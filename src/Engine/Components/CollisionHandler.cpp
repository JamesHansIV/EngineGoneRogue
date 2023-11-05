#include "CollisionHandler.h"

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler(){
}

bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b){
    if (a.x + a.w < b.x)
        return false;
    if (b.x + b.w < a.x)
        return false;

    if (a.y + a.h < b.y)
        return false;
    if (b.y + b.h < a.y)
        return false;

    return true;
}
