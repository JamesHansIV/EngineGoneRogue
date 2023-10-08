#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



//The window we'll be rendering to
SDL_Window* gWindow = NULL;
    
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    gHelloWorld = SDL_LoadBMP("src/test.bmp");
    if (gHelloWorld == NULL) {
        printf( "Unable to load image %s! SDL Error: %s\n", "src/test.bmp", SDL_GetError() );
        success = false;
    }

    return success;
}

void close() {
    SDL_FreeSurface( gHelloWorld);
    gHelloWorld = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main() {

    if (!init()) {
        std::cout << "Failed to initialize!" << std::endl;
    } else {
        if (!loadMedia()) {
            std::cout << "Failed to load media!" << std::endl;
        } else {

            bool quit = false;
            SDL_Event e;

            while(!quit) {
                while(SDL_PollEvent(&e) != 0) {

                    // quit event
                    if (e.type == SDL_QUIT) {
                        std::cout << "Quiting..." << std::endl;
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        std::cout << (char)e.key.keysym.sym << " pressed..." << std::endl;
                    } else if (e.type == SDL_KEYUP) {
                        std::cout << (char)e.key.keysym.sym <<  " released..." << std::endl;
                    }

                    // map image
                    SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
                    SDL_UpdateWindowSurface(gWindow);
                }
            }
        }
    }

    close();

    return 0;
}