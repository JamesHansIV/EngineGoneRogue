#include "Engine/Engine.h"
#include <iostream>

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