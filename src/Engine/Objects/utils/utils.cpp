#include "utils.h"

SDL_Rect DrawBar(Position pos, Size size) {
    int experience_bar_container_width = size.w;

    if (experience_bar_container_width <= 0) {
        experience_bar_container_width = 0;
    }

    int const experience_bar_container_x = pos.x;
    int const experience_bar_container_y = pos.y;

    SDL_Rect experience_bar_container_rect;
    experience_bar_container_rect.x = experience_bar_container_x;
    experience_bar_container_rect.y = experience_bar_container_y;
    experience_bar_container_rect.w = experience_bar_container_width;
    experience_bar_container_rect.h = 10;

    return experience_bar_container_rect;
}
