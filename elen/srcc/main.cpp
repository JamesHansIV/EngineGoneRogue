#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include "Core/Engine.h"

int main(int argc, char* args[])
{
    Engine::GetInstance()->Init();

    while(Engine::GetInstance()->IsRunning())
    {
        Engine::GetInstance()->Events();
        Engine::GetInstance()->Update(0.0);
        Engine::GetInstance()->Render();
    }

    Engine::GetInstance()->Clean();
}