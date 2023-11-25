#include "Game.h"
#include <cstdlib>
#include "Engine/Events/EventListener.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Enemy.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Objects/Player.h"
#include "Engine/Renderer/Renderer.h"

Player* player = nullptr;
Enemy* enemy1 = nullptr;
Enemy* enemy2 = nullptr;
Enemy* enemy3 = nullptr;
Enemy* enemy4 = nullptr;
Enemy* enemy5 = nullptr;
Enemy* enemy6 = nullptr;
Enemy* enemy7 = nullptr;
Enemy* enemy8 = nullptr;
std::vector<Collider*> colliders;

int max_tick_interval = 100;
int cur_enemy_generation_interval = 500;

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    Renderer::GetInstance()->AddTexture(
        "player_move_up", "../assets/textures/spritesheets/player-move-up.png");

    Renderer::GetInstance()->AddTexture("door-open", "../assets/textures/spritesheets/door2-open.png");
    Renderer::GetInstance()->AddTexture("door-close", "../assets/textures/spritesheets/door2-close.png");

    Renderer::GetInstance()->AddTexture(
        "player",
        "../assets/textures/spritesheets/player-sheet.png");
    Renderer::GetInstance()->AddTexture(
        "player_move_down",
        "../assets/textures/spritesheets/player-move-down.png");
    Renderer::GetInstance()->AddTexture(
        "player_move_right",
        "../assets/textures/spritesheets/player-move-right.png");
    Renderer::GetInstance()->AddTexture(
        "player_move_right2",
        "../assets/textures/spritesheets/player-move-right2.png");
    Renderer::GetInstance()->AddTexture(
        "enemy1",
        "../assets/textures/spritesheets/enemy1_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture(
        "enemy2",
        "../assets/textures/spritesheets/enemy2_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture(
        "enemy3",
        "../assets/textures/spritesheets/enemy3_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture(
        "enemy4",
        "../assets/textures/spritesheets/enemy4_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture(
        "enemy5",
        "../assets/textures/spritesheets/enemy5_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture(
        "boss", "../assets/textures/spritesheets/boss_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture(
        "tilemap",
        "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    Renderer::GetInstance()->AddTexture("player_run",
                                        "../assets/textures/Run.png");
    Renderer::GetInstance()->AddTexture(
        "player_dead", "../assets/textures/spritesheets/player-dead.png");
    Renderer::GetInstance()->AddTexture(
        "projectile",
        "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Firearms/Bullets/"
        "spr_bullet3.png");
    Renderer::GetInstance()->AddTexture(
        "melee",
        "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Melee/"
        "spr_sword_03.png");
    Renderer::GetInstance()->AddTexture(
        "gun",
        "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Firearms/"
        "spr_weapon06.png");

    m_Objects = Application::m_Rooms["room1"];

    for (auto& obj : m_Objects) {
        if (auto* collider = dynamic_cast<Collider*>(obj)) {
            ColliderHandler::GetInstance()->AddCollider(collider);
        }
    }

    Properties props_e("open", {0, 0, 16, 32}, {300, 300, 16, 32}, 0, "entrance");
    auto* entrance = new Entrance(props_e);

    Properties props_p("player", {0, 0, 18, 18}, {0, 0, 30, 30}, 0, "player");
    player = new Player(props_p);

    Properties props1("enemy5", {0, 0, 16, 16}, {200, 200, 36, 36}, 0,
                      "enemy1");
    enemy1 = new Enemy(props1, 150, 150);

    Properties props2("enemy5", {0, 0, 16, 16}, {300, 260, 36, 36}, 0,
                      "enemy2");
    enemy2 = new Enemy(props2, 150, 150);

    Properties props3("enemy5", {0, 0, 16, 16}, {500, 200, 36, 36}, 0,
                      "enemy3");
    enemy3 = new Enemy(props3, 150, 150);

    Properties props4("enemy5", {0, 0, 16, 16}, {600, 367, 36, 36}, 0,
                      "enemy4");
    enemy4 = new Enemy(props4, 150, 150);

    Properties props5("enemy5", {0, 0, 16, 16}, {700, 300, 36, 36}, 0,
                      "enemy5");
    enemy5 = new Enemy(props5, 150, 150);

    Properties props6("enemy5", {0, 0, 16, 16}, {600, 150, 36, 36}, 0,
                      "enemy6");
    enemy6 = new Enemy(props6, 150, 150);

    m_Objects.push_back(enemy1);
    m_Objects.push_back(enemy2);
    m_Objects.push_back(enemy3);
    m_Objects.push_back(enemy4);
    m_Objects.push_back(enemy5);
    m_Objects.push_back(enemy6);
    m_Objects.push_back(entrance);
    ColliderHandler::GetInstance()->AddCollider(player);
    ColliderHandler::GetInstance()->AddCollider(enemy1);
    ColliderHandler::GetInstance()->AddCollider(enemy2);
    ColliderHandler::GetInstance()->AddCollider(enemy3);
    ColliderHandler::GetInstance()->AddCollider(enemy4);
    ColliderHandler::GetInstance()->AddCollider(enemy5);
    ColliderHandler::GetInstance()->AddCollider(enemy6);
    ColliderHandler::GetInstance()->AddCollider(entrance);

    srand(time(nullptr));

    Renderer::GetInstance()->SetCameraTarget(player);
}

void Game::Events() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                return;
            case SDL_KEYDOWN:
                InputChecker::SetKeyPressed(event.key.keysym.sym, true);
                player->OnKeyPressed(event);
                break;
            case SDL_KEYUP:
                InputChecker::SetKeyPressed(event.key.keysym.sym, false);
                player->OnKeyReleased(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                InputChecker::SetMouseButtonPressed(event.button.button, true);
                break;
            case SDL_MOUSEBUTTONUP:
                InputChecker::SetMouseButtonPressed(event.button.button, false);
                break;
            case SDL_MOUSEMOTION:
                InputChecker::UpdateMousePosition(event.motion.x,
                                                  event.motion.y);
                break;
            case SDL_MOUSEWHEEL:
                InputChecker::SetMouseWheelDirection(event.wheel.y);
                break;
        }
    }
}

void Game::Update(float dt) {
    if (player->IsMarkedForDeletion()) {
        player->Clean();
        delete player;
    }
    player->Update(dt);
    if (player->GetState().HasState(ObjectState::ToBeDestroyed)) {
        DeleteObject(player);
    }
    for (auto it = m_Objects.begin(); it != m_Objects.end();) {
        auto* enemy = dynamic_cast<Enemy*>(*it);  // Cast to Enemy type
        if (enemy != nullptr) {
            enemy->SetTarget(player);
        }
        (*it)->Update(dt);
        if ((*it)->GetState().HasState(
                ObjectState::
                    ToBeDestroyed))  // Check if it's an Enemy and marked for deletion
        {
            DeleteObject(*it);
        } else {
            ++it;
        }
    }
    ColliderHandler::GetInstance()->HandleCollisions();

    m_tick++;
    // if (m_tick % cur_enemy_generation_interval == 0) {
    //   float generated_x = rand() % 500 + 200;
    //   float generated_y = rand() % 300 + 20;
    //   Properties generated_props("enemy5",{0, 0, 16, 16}, {generated_x, generated_y, 36, 36});
    //   auto* generated_enemy = new Enemy(generated_props, 300, 300);
    //   ColliderHandler::GetInstance()->AddCollider(generated_enemy);
    //   m_Objects.push_back(generated_enemy);
    // }
    // if (max_tick_interval < cur_enemy_generation_interval) {
    //   cur_enemy_generation_interval -= 20;
    // }
}

void Game::Render() {
    Renderer::GetInstance()->RenderClear();
    for (auto* obj : m_Objects) {
        obj->Draw();
    }
    player->Draw();
    Renderer::GetInstance()->Render();
}

void Game::DeleteObject(GameObject* obj) {
    auto it = std::find(m_Objects.begin(), m_Objects.end(), obj);
    if (it != m_Objects.end()) {
        SDL_Log("found obj in m_Objects: %s", (*it)->GetID().c_str());
        m_Objects.erase(it);
        SDL_Log("m_Objects size: %lu", m_Objects.size());
    }
    obj->Clean();
    delete obj;
    obj = nullptr;
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif
