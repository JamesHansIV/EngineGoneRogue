#include "Game.h"
#include <cstdlib>
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Characters/CircleShotEnemy.h"
#include "Engine/Objects/Characters/Dog.h"
#include "Engine/Objects/Characters/Goblin.h"
#include "Engine/Objects/Characters/HelixEnemy.h"
#include "Engine/Objects/Characters/Mage.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Characters/Skeleton.h"
#include "Engine/Objects/Characters/Slime.h"
#include "Engine/Objects/Environment/Entrance.h"
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
Enemy* enemy9 = nullptr;
Enemy* enemy10 = nullptr;

std::vector<Collider*> colliders;

int max_tick_interval = 100;
int cur_enemy_generation_interval = 500;

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    Renderer::GetInstance()->AddTexture(
        "door-open", "../assets/textures/spritesheets/door2-open.png");
    Renderer::GetInstance()->AddTexture(
        "door-close", "../assets/textures/spritesheets/door2-close.png");

    Renderer::GetInstance()->AddTexture(
        "weapons", "../assets/textures/spritesheets/weapons.png");
    Renderer::GetInstance()->AddTexture(
        "player", "../assets/textures/spritesheets/player-sheet.png");
    Renderer::GetInstance()->AddTexture(
        "player-dodge", "../assets/textures/spritesheets/player-dodge.png");
    Renderer::GetInstance()->AddTexture(
        "enemies", "../assets/textures/spritesheets/enemies.png");
    Renderer::GetInstance()->AddTexture(
        "boss", "../assets/textures/spritesheets/boss_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("player_run",
                                        "../assets/textures/Run.png");
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

    Properties props_e("open", {0, 0, 16, 32}, {300, 300, 16, 32}, 0,
                       "entrance");
    auto* entrance = new Entrance(props_e);

    Properties props_p("player", {0, 0, 18, 18}, {0, 0, 30, 30}, 0, "player");
    player = new Player(props_p);

    Properties props1("enemies", {0, 1, 16, 16}, {200, 200, 36, 36}, 0,
                      "enemy1");
    enemy1 = new Slime(props1, 150, 150);

    Properties props2("enemies", {0, 1, 16, 16}, {300, 260, 36, 36}, 0,
                      "enemy2");
    enemy2 = new Slime(props2, 150, 150);

    Properties props3("enemies", {0, 1, 16, 16}, {500, 200, 36, 36}, 0,
                      "enemy3");
    enemy3 = new Slime(props3, 150, 150);

    Properties props4("enemies", {0, 1, 16, 16}, {600, 367, 36, 36}, 0,
                      "enemy4");
    enemy4 = new Slime(props4, 150, 150);

    Properties props5("enemies", {0, 1, 16, 16}, {700, 300, 36, 36}, 0,
                      "enemy5");
    enemy5 = new CircleShotEnemy(props5, 150, 150);

    Properties props6("enemies", {6, 2, 16, 16}, {600, 150, 36, 36}, 0,
                      "enemy6");
    enemy6 = new Mage(props6, 150, 150);

    Properties props7("enemies", {6, 2, 16, 16}, {500, 300, 36, 36}, 0,
                      "enemy7");
    enemy7 = new Dog(props7, 200, 200);

    Properties props8("enemies", {9, 2, 16, 16}, {600, 300, 36, 36}, 0,
                      "enemy8");
    enemy8 = new Skeleton(props8, 175, 175);

    Properties props9("enemies", {9, 2, 16, 16}, {550, 300, 36, 36}, 0,
                      "enemy9");
    enemy9 = new Goblin(props9, 200, 200);

    Properties props10("enemies", {9, 2, 16, 16}, {400, 300, 36, 36}, 0,
                       "enemy10");
    enemy10 = new HelixEnemy(props10, 200, 200);

    m_Objects.push_back(enemy1);
    m_Objects.push_back(enemy2);
    m_Objects.push_back(enemy3);
    m_Objects.push_back(enemy4);
    m_Objects.push_back(enemy5);
    m_Objects.push_back(enemy6);
    m_Objects.push_back(enemy7);
    m_Objects.push_back(enemy8);
    m_Objects.push_back(enemy9);
    m_Objects.push_back(enemy10);
    m_Objects.push_back(entrance);
    ColliderHandler::GetInstance()->AddCollider(player);
    ColliderHandler::GetInstance()->AddCollider(enemy1);
    ColliderHandler::GetInstance()->AddCollider(enemy2);
    ColliderHandler::GetInstance()->AddCollider(enemy3);
    ColliderHandler::GetInstance()->AddCollider(enemy4);
    ColliderHandler::GetInstance()->AddCollider(enemy5);
    ColliderHandler::GetInstance()->AddCollider(enemy6);
    ColliderHandler::GetInstance()->AddCollider(enemy7);
    ColliderHandler::GetInstance()->AddCollider(enemy8);
    ColliderHandler::GetInstance()->AddCollider(enemy9);
    ColliderHandler::GetInstance()->AddCollider(enemy10);
    ColliderHandler::GetInstance()->AddCollider(entrance);

    srand(time(nullptr));

    Renderer::GetInstance()->SetCameraTarget(player);
}

void Game::Events() {
    SDL_Event event;
    UserEvent event_wrapper;
    event_wrapper.SetSDLEvent(&event);
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                return;
            case SDL_KEYDOWN:
                InputChecker::SetKeyPressed(event.key.keysym.sym, true);
                //player->OnKeyPressed(event);
                break;
            case SDL_KEYUP:
                InputChecker::SetKeyPressed(event.key.keysym.sym, false);
                //player->OnKeyReleased(event);
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
            default:
                break;
        }
        player->HandleEvent(&event_wrapper);
    }
}

void Game::Update(float dt) {
    if (player->MarkedForDeletion()) {
        player->Clean();
        delete player;
    }
    ColliderHandler::GetInstance()->HandleCollisions();

    player->Update(dt);
    if (player->MarkedForDeletion()) {
        DeleteObject(player);
    }
    for (auto it = m_Objects.begin(); it != m_Objects.end();) {
        auto* enemy = dynamic_cast<Enemy*>(*it);  // Cast to Enemy type
        if (enemy != nullptr) {
            enemy->SetTarget(player);
        }
        (*it)->Update(dt);
        if ((*it)
                ->MarkedForDeletion())  // Check if it's an Enemy and marked for deletion
        {
            SDL_Log("deleted obj: %s", (*it)->GetID().c_str());
            DeleteObject(*it);
        } else {
            ++it;
        }
    }

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

void Game::AddObject(GameObject* obj) {
    SDL_Log("Adding object: %s", obj->GetID().c_str());
    m_Objects.push_back(obj);
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
