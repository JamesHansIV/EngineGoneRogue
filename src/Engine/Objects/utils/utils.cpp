#include "utils.h"

SDL_Rect DrawBar(Position pos, Size size) {
    int bar_container_width = size.w;

    if (bar_container_width <= 0) {
        bar_container_width = 0;
    }

    int const bar_container_x = pos.x;
    int const bar_container_y = pos.y;

    SDL_Rect bar_container_rect;
    bar_container_rect.x = bar_container_x;
    bar_container_rect.y = bar_container_y;
    bar_container_rect.w = bar_container_width;
    bar_container_rect.h = size.h;

    return bar_container_rect;
}
