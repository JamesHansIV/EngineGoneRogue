#include "Game.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "Engine/Objects/Warrior.h"
#include "Events/EventListener.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/InputChecker.h"

Warrior* player = nullptr;
std::vector<Projectile*> projectiles;

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
}

void Game::Update(float dt) {
    player->Update(dt);
    Properties projectile_props("projectile", player->getTransform()->X + 50, player->getTransform()->Y + 30, 723, 724, 15, 15);
    if (InputChecker::isKeyPressed(SDLK_SPACE))
    {
        Projectile* projectile = nullptr;
        if(player->getFlip() == SDL_FLIP_HORIZONTAL)
        {
            projectile = new Projectile(projectile_props, 50, 1.0, 180);
        }
        else
        {
            projectile = new Projectile(projectile_props, 50, 1.0, 0);
        }
        projectiles.push_back(projectile);
        InputChecker::setKeyPressed(SDLK_SPACE,false);
    } 
    for (auto projectile : projectiles) {
        projectile->Update(dt);
    }  
}


void Game::Render() {
    Renderer::GetInstance()->RenderClear();
    m_Map->Draw();
    player->Draw();
    for (auto projectile : projectiles) {
        projectile->Draw();
    }

    Renderer::GetInstance()->Render();
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif