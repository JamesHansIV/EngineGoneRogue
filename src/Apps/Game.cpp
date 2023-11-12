#include "Game.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Objects/Player.h"
#include "Engine/Events/EventListener.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Input/InputChecker.h"

Player* player = nullptr;
Player* player2 = nullptr;
Player* player3 = nullptr;
std::vector<Projectile*> projectiles;
std::vector<GameObject*> colliders;

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    Renderer::GetInstance()->AddTexture("player", "../assets/textures/spritesheets/player-front-idle.png");
    Renderer::GetInstance()->AddTexture("tilemap", "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    Renderer::GetInstance()->AddTexture("player_run", "../assets/textures/Run.png");
    Renderer::GetInstance()->AddTexture("projectile", "../assets/textures/dot_PNG2.png");

    m_Objects = Application::m_Rooms["room1"];

    Properties props("player", {0, 0, 18, 16}, {0, 0, 18, 16});
    player = new Player(props);

    Properties props2("player",{0, 0, 18, 16}, {100, 128, 18, 16});
    player2 = new Player(props2);

    Properties props3("player", {0, 0, 18, 16}, {700, 360, 16});
    Properties props4("player", {0, 0, 18, 16}, {300, 278, 18, 16});
    Properties props5("player", {0, 0, 18, 16}, {200, 389, 18, 16});
    player3 = new Player(props3);
    // player4 = new Player(props4);
    // player5 = new Player(props5);
    // player4 = new Player(props4);
    // player5 = new Player(props5);

    colliders.push_back(player2);
    colliders.push_back(player3);

    GetEventManager().AddListener(*player);

    Renderer::GetInstance()->SetCameraTarget(player);
}


void Game::Update(float dt) {
    player->Update(dt,colliders);
    int const player_x = player->GetMidPointX() - Renderer::GetInstance()->GetCameraX();
    int const player_y = player->GetMidPointY() - Renderer::GetInstance()->GetCameraY();

    SDL_Log("%d, %d", player_x , player_y );
    int const mouse_x = InputChecker::GetMouseX();
    int const mouse_y = InputChecker::GetMouseY();

    SDL_Log("%d, %d", mouse_x , mouse_y );

    // Calculate the angle between the mouse and the player
    float const delta_x = mouse_x - player_x;
    float const delta_y = mouse_y - player_y;
    float angle = atan2(delta_y, delta_x) * (180.0 / M_PI);
    // Convert the angle range from -180 to 180 to 0 to 360
    if (angle < 0) angle += 360.0F;
    if (angle < 0) angle += 360.0F;

    SDL_Log("%f", angle);

    Properties projectile_props("projectile", {0, 0, 723, 724}, {player->GetMidPointX(), player->GetMidPointY(), 15, 15});
    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT))
    {
        Projectile* projectile = nullptr;
        projectile = new Projectile(projectile_props, 50, 1.0, angle);
        projectiles.push_back(projectile);
        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    }
    for (auto *projectile : projectiles) {
        projectile->Update(dt, colliders);
    }
}

void Game::Render() {
    Renderer::GetInstance()->RenderClear();
    // m_Map->Draw();
    for (auto *obj : m_Objects) {
        obj->Draw();
    }
    player->Draw();
    player2->Draw();
    player3->Draw();
    for (auto *projectile : projectiles) {
        projectile->Draw();
    }

    Renderer::GetInstance()->Render();
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif
