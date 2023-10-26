#include "Engine.h"
#include "../Graphics/TextureManager.h"

#include "../Objects/Warrior.h"
#include "../Objects/GameObject.h"
#include "InputChecker.h"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "../utils.h"

#include <unistd.h>

Engine* Engine::m_Instance = nullptr;
Warrior* player = nullptr;

bool Engine::Init(){

    if(SDL_Init(SDL_INIT_VIDEO)!=0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!= 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    m_Window = SDL_CreateWindow("Soft Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(m_Window == nullptr){
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_Renderer == nullptr){
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }
    //TODO: note that the cwd is <projectDir>/build instead of <projectDir>.
    //      Set a working directory path macro to use absolute file paths
    m_CurrentID = 0;

    TextureManager::GetInstance()->AddTexture("player", "../assets/textures/Idle.png");
    TextureManager::GetInstance()->AddTexture("tilemap", "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    m_Map = new Map("tilemap");
    if (!m_Map->LoadMap("../assets/maps/tiny_dungeon1.txt")) {
        SDL_Log("Failed to load map\n");
        return false;
    }
    
    Properties props("player", 0, 0, 136, 96);
    player = new Warrior(props);

    //AddObject(player);

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer2_Init(m_Renderer);

    m_EventHandler.addListener(*player);
    Transform tf;
    tf.Log();
    return m_IsRunning = true;
}

void Engine::Update(float dt){
    player->Update(0);

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::End();
    }

}

void Engine::Render(){
    
    
    ImGui::Render();
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    m_Map->Draw();
    player->Draw();
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
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
        }
    }
    m_EventHandler.handleEvent(event);
}

bool Engine::Clean(){
    //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}

void Engine::Quit(){
    m_IsRunning = false;
}
