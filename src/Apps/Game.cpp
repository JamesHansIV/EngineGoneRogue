#include "Game.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Objects/Player.h"
#include "Engine/Events/EventListener.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Input/InputChecker.h"

Player* player = nullptr;
Player* player2 = nullptr;
Player* player3 = nullptr;
Player* player4 = nullptr;
Player* player5 = nullptr;
std::vector<Projectile*> projectiles;
std::vector<GameObject*> colliders;

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    Renderer::GetInstance()->AddTexture("player", "../assets/textures/Idle.png");
    Renderer::GetInstance()->AddTexture("tilemap", "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    Renderer::GetInstance()->AddTexture("player_run", "../assets/textures/Run.png");
    Renderer::GetInstance()->AddTexture("projectile", "../assets/textures/dot_PNG2.png");

    m_Objects = Application::m_Rooms["room1"];

    Properties props("player", {0, 0, 136, 96}, {0, 0, 136, 96});
    player = new Player(props);

    Properties props2("player",{0, 0, 136, 96}, {100, 128, 136, 96});
    player2 = new Player(props2);

    Properties props3("player", {0, 0, 136, 96}, {700, 360, 136, 96});
    Properties props4("player", {0, 0, 136, 96}, {300, 278, 136, 96});
    Properties props5("player", {0, 0, 136, 96}, {200, 389, 136, 96});
    player3 = new Player(props3);
    player4 = new Player(props4);
    player5 = new Player(props5);

    colliders.push_back(player2);
    colliders.push_back(player3);
    colliders.push_back(player4);
    colliders.push_back(player5);

    GetEventManager().addListener(*player);

    Renderer::GetInstance()->SetCameraTarget(player);
}


void Game::Update(float dt) {
    player->Update(dt,colliders);
    int playerX = player->GetMidPointX() - Renderer::GetInstance()->GetCameraX();
    int playerY = player->GetMidPointY() - Renderer::GetInstance()->GetCameraY();

    SDL_Log("%d, %d", playerX , playerY );
    int mouseX = InputChecker::getMouseX();
    int mouseY = InputChecker::getMouseY();

    SDL_Log("%d, %d", mouseX , mouseY );

    // Calculate the angle between the mouse and the player
    float deltaX = mouseX - playerX;
    float deltaY = mouseY - playerY;
    float angle = atan2(deltaY, deltaX) * (180.0 / M_PI);
    // Convert the angle range from -180 to 180 to 0 to 360
    if (angle < 0) {
        angle += 360.0f;
    }

    SDL_Log("%f", angle);

    Properties projectile_props("projectile", {0, 0, 723, 724}, {player->GetMidPointX(), player->GetMidPointY(), 15, 15});
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
    for (auto obj : m_Objects) {
        obj->Draw();
    }
    player->Draw();
    player2->Draw();
    player3->Draw();
    player4->Draw();
    player5->Draw();
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
