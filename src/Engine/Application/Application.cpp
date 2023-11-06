#include "Application.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Warrior.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Input/InputChecker.h"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "utils.h"

#include <unistd.h>

Application* Application::m_Instance = nullptr;

Application::Application() : m_ProjectName("test_project") {

    if(SDL_Init(SDL_INIT_VIDEO)!=0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!= 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        assert(false);
    }

    m_Window = SDL_CreateWindow("Engine Gone Rogue", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(m_Window == nullptr){
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        assert(false);
    }

    m_Instance = this;

    //TODO: note that the cwd is <projectDir>/build instead of <projectDir>.
    //      Set a working directory path macro to use absolute file paths
    Renderer::GetInstance()->Init();

    m_IsRunning = true;
}

void Application::Events(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_EventManager.handleEvent(event);
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                return;
            case SDL_KEYDOWN:
                InputChecker::setKeyPressed(event.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                InputChecker::setKeyPressed(event.key.keysym.sym, false);
                break;
            case SDL_MOUSEBUTTONDOWN:
                InputChecker::setMouseButtonPressed(event.button.button, true);
                break;
            case SDL_MOUSEBUTTONUP:
                InputChecker::setMouseButtonPressed(event.button.button, false);
                break;
            case SDL_MOUSEMOTION:
                InputChecker::updateMousePosition(event.motion.x, event.motion.y);
                break;
        }
    }
}

void Application::Run() {
    while (m_IsRunning) {
        Events();
        Update(0.5);
        Render();
    }
    Clean();
}

bool Application::Clean(){
    Renderer::GetInstance()->Destroy();

    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}

void Application::Quit(){
    m_IsRunning = false;
}

