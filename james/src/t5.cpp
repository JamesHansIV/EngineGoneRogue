#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
    
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

SDL_Surface* gStretchedSurface = NULL;

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

SDL_Surface* loadSurface(std::string path) {
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

bool loadMedia() {
    bool success = true;

    gStretchedSurface = SDL_LoadBMP("src/t5.bmp");
    if (gStretchedSurface == NULL) {
        printf( "Unable to load image %s! \nSDL Error: %s\n", "src/t5.bmp", SDL_GetError() );
        success = false;
    }

    return success;
}

void close() {
    SDL_FreeSurface( gStretchedSurface);
    gStretchedSurface = NULL;

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
                    }

                    else if (e.type == SDL_KEYDOWN) {
                        std::cout << (char)e.key.keysym.sym << " pressed..." << std::endl;
                    } else if (e.type == SDL_KEYUP) {
                        std::cout << (char)e.key.keysym.sym <<  " released..." << std::endl;
                    }


                    //Apply the image stretched
                    SDL_Rect stretchRect;
                    stretchRect.x = 0;
                    stretchRect.y = 0;
                    stretchRect.w = SCREEN_WIDTH;
                    stretchRect.h = SCREEN_HEIGHT;
                    SDL_BlitScaled( gStretchedSurface, NULL, gScreenSurface, &stretchRect );

                    // map image
                    SDL_BlitSurface(gStretchedSurface, NULL, gScreenSurface, NULL);
                    SDL_UpdateWindowSurface(gWindow);
                }
            }
        }
    }

    close();

    return 0;
}