#include "globals.h"

class Window {
    public:
    Window(SDL_Window* window, int h = SCREEN_WIDTH, int w = SCREEN_HEIGHT);
    ~Window();
    void createRenderer();
    SDL_Event* pollEvent(SDL_Event* event);

    private:
    SDL_Window* window;
    int height;
    int width;
};