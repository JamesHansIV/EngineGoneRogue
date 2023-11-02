#include "Game.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "Engine/Objects/Warrior.h"
#include "Events/EventListener.h"
#include "Engine/Objects/Projectile.h"

Warrior* player = nullptr;
Projectile* projectile = nullptr;

Game::Game() {
    ImGui::CreateContext();
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();
    ImGui_ImplSDL2_InitForSDLRenderer(GetWindow(), renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    Renderer::GetInstance()->AddTexture("player", "../assets/textures/Idle.png");
    Renderer::GetInstance()->AddTexture("tilemap", "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    Renderer::GetInstance()->AddTexture("player_run", "../assets/textures/Run.png");
    Renderer::GetInstance()->AddTexture("projectile", "../assets/textures/dot_PNG2.png");

    m_Map = new Map("tilemap");
    if (!m_Map->LoadMap("../assets/maps/tiny_dungeon1.txt")) {
        SDL_Log("Failed to load map\n");
        assert(false);
    }

    Properties props("player", 0, 0, 136, 96, 136, 96);
    player = new Warrior(props);
    GetEventManager().addListener(*player);

    Properties projectile_props("projectile", 0, 0, 723, 724, 25, 25);
    projectile = new Projectile(projectile_props);
}

void Game::Update(float dt) {
    player->Update(dt);
    projectile->Update(dt);
}


void Game::Render() {
    Renderer::GetInstance()->RenderClear();
    m_Map->Draw();
    player->Draw();
    projectile->Draw();
    Renderer::GetInstance()->Render();
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif