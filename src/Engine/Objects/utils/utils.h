#pragma once

#include "Engine/Renderer/Renderer.h"
#include "SDL2/SDL_rect.h"

struct Position {
    int x;
    int y;
};

struct Size {
    int w;
    int h;
};

SDL_Rect DrawBar(Position pos, Size size);
