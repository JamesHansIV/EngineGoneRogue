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

    int playerX = player->GetMidPointX();
    int playerY = player->GetMidPointY();

    int mouseX = InputChecker::getMouseX();
    int mouseY = InputChecker::getMouseY();

    // Calculate the angle between the mouse and the player
    float deltaX = mouseX - playerX;
    float deltaY = mouseY - playerY;
    float angle = atan2(deltaY, deltaX) * (180.0 / M_PI);
    // Convert the angle range from -180 to 180 to 0 to 360
    if (angle < 0) {
        angle += 360.0f;
    }

    SDL_Log("%f", angle);

    Properties projectile_props("projectile", player->GetMidPointX(), player->GetMidPointY(), 723, 724, 15, 15);
    if (InputChecker::isMouseButtonPressed(SDL_BUTTON_LEFT))
    {
        Projectile* projectile = nullptr;
        projectile = new Projectile(projectile_props, 50, 1.0, angle);
        projectiles.push_back(projectile);
        InputChecker::setMouseButtonPressed(SDL_BUTTON_LEFT, false);
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