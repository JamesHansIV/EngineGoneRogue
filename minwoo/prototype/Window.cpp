#include "Window.h"

Window::Window(SDL_Window* window, int h, int w) : window(window), height(h), width(w) {
    window = SDL_CreateWindow( "Engine Gone Rogue", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
    if( window == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    }
}

Window::~Window() {
    SDL_DestroyWindow( window );
    window = NULL;
}

void Window::createRenderer() {
    gRenderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if( gRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

SDL_Event* Window::pollEvent(SDL_Event* event) {
    while (SDL_PollEvent(event) != 0) {

    }
    return event;
}