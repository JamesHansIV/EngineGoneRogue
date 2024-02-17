#include "Game.h"
#include <cstdlib>
#include <vector>
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Characters/CircleShotEnemy.h"
#include "Engine/Objects/Characters/Dog.h"
#include "Engine/Objects/Characters/EnemyStats.h"
#include "Engine/Objects/Characters/Goblin.h"
#include "Engine/Objects/Characters/HelixEnemy.h"
#include "Engine/Objects/Characters/Mage.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Characters/Skeleton.h"
#include "Engine/Objects/Characters/Slime.h"
#include "Engine/Objects/Chests/Chest.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/HealthPotion.h"
#include "Engine/Objects/WeaponInventory.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"
#include "SDL2/SDL_log.h"

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

int max_tick_interval = 500;
int cur_enemy_generation_interval = 5000;

const EnemyStats kDefaultEnemyStats = {
    100,  // health
    1,    // damage
    10,   // speed
    150,  // range
    160,  // perception width
    150,  // perception height
    15,   // xp
};
const RangedEnemyStats kDefaultRangedEnemyStats = {
    kDefaultEnemyStats,  // enemy stats
    3000,                // fireInterval
    4                    // spread};
};

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
    Renderer::GetInstance()->AddTexture("arrow",
                                        "../assets/textures/BulletHell/PURPLE/"
                                        "Weapons/weapons/Bow-Arrow/"
                                        "spr_arrow1.png");
    Renderer::GetInstance()->AddTexture(
        "bow",
        "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Bow-Arrow/"
        "spr_bow41.png");
    Renderer::GetInstance()->AddTexture(
        "melee",
        "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Melee/"
        "spr_sword_03.png");
    Renderer::GetInstance()->AddTexture(
        "pistol",
        "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Firearms/"
        "spr_weapon03.png");
    Renderer::GetInstance()->AddTexture(
        "uzi",
        "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Firearms/"
        "spr_weapon05.png");
    Renderer::GetInstance()->AddTexture(
        "sniper",
        "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Firearms/"
        "spr_weapon06.png");

    Renderer::GetInstance()->AddTexture(
        "healthpotion", "../assets/textures/spritesheets/lifepotion.png");

    Renderer::GetInstance()->AddTexture(
        "wooden_chest_idle",
        "../assets/textures/BulletHell/PURPLE/Others/Chest/spr_chest1.png");

    Renderer::GetInstance()->AddTexture(
        "wooden_chest_opening", "../assets/textures/spritesheets/chest.png");

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
    enemy1 = new Slime(props1, kDefaultEnemyStats);

    Properties props2("enemies", {0, 1, 16, 16}, {300, 260, 36, 36}, 0,
                      "enemy2");
    enemy2 = new Slime(props2, kDefaultEnemyStats);

    Properties props3("enemies", {0, 1, 16, 16}, {500, 200, 36, 36}, 0,
                      "enemy3");
    enemy3 = new Slime(props3, kDefaultEnemyStats);

    Properties props4("enemies", {0, 1, 16, 16}, {600, 367, 36, 36}, 0,
                      "enemy4");
    enemy4 = new Slime(props4, kDefaultEnemyStats);

    Properties props5("enemies", {0, 1, 16, 16}, {700, 300, 36, 36}, 0,
                      "enemy5");
    // specific stats replaced with defaults for easy refactor. can be changed later
    // specific enemy stats changed in commit on 2/4.
    enemy5 = new CircleShotEnemy(props5, kDefaultRangedEnemyStats);

    Properties props6("enemies", {6, 2, 16, 16}, {600, 150, 36, 36}, 0,
                      "enemy6");
    enemy6 = new Mage(props6, kDefaultRangedEnemyStats);

    Properties props7("enemies", {6, 2, 16, 16}, {500, 300, 36, 36}, 0,
                      "enemy7");
    enemy7 = new Dog(props7, kDefaultRangedEnemyStats);

    Properties props8("enemies", {9, 2, 16, 16}, {600, 300, 36, 36}, 0,
                      "enemy8");
    enemy8 = new Skeleton(props8, kDefaultRangedEnemyStats);

    Properties props9("enemies", {9, 2, 16, 16}, {550, 300, 36, 36}, 0,
                      "enemy9");
    enemy9 = new Goblin(props9, kDefaultRangedEnemyStats);

    Properties props10("enemies", {9, 2, 16, 16}, {400, 300, 36, 36}, 0,
                       "enemy10");
    enemy10 = new HelixEnemy(props10, kDefaultRangedEnemyStats);

    Properties props11("healthpotion", {1, 1, 16, 16}, {250, 150, 25, 25}, 0,
                       "healthpotion");
    auto* healthpotion = new HealthPotion(props11, 20);

    Properties props12("healthpotion", {1, 1, 16, 16}, {550, 400, 25, 25}, 0,
                       "healthpotion2");
    auto* healthpotion2 = new HealthPotion(props12, 20);

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
    m_Objects.push_back(healthpotion);
    m_Objects.push_back(healthpotion2);
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
    ColliderHandler::GetInstance()->AddCollider(healthpotion);
    ColliderHandler::GetInstance()->AddCollider(healthpotion2);

    srand(time(nullptr));

    m_WeaponInventory = new WeaponInventory(player->GetPlayerWeapons());
    m_WeaponInventory->SetSelectedWeapon(player->GetCurrentWeapon());

    Renderer::GetInstance()->SetCameraTarget(player);
    m_GameEventManager = new GameEventManager(player, m_Objects);

    m_HeadsUpDisplay = new HUD();

    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    SDL_SetCursor(cursor);
}

void Game::Events() {
    m_GameEventManager->HandleEvents();
}

void Game::GenerateRandomEnemyIfNeeded() {
    // We probably want a wave multipler. Idea of waves.
    // Have dedicated stats for each enemy
    // Have a wave manager that will spawn enemies
    // have a wave multiplier that will increase the stats of the enemies
    // Auto generate enemies
    if ((SDL_GetTicks() - m_last_enemy_spawn_time) >=
        cur_enemy_generation_interval) {
        m_last_enemy_spawn_time = SDL_GetTicks();
        // Generate random x and y coordinates
        float const generated_x = rand() % 500 + 200;
        float const generated_y = rand() % 300 + 20;
        Properties generated_props("enemies", {0, 1, 16, 16},
                                   {generated_x, generated_y, 36, 36}, 0,
                                   "enemy1");
        // Generate random enemy type
        int const enemy_type = rand() % 7;
        Enemy* generated_enemy = nullptr;
        switch (enemy_type) {
            case 0:
                generated_enemy =
                    new Slime(generated_props, kDefaultEnemyStats);
                break;
            case 1:
                generated_enemy = new CircleShotEnemy(generated_props,
                                                      kDefaultRangedEnemyStats);
                break;
            case 2:
                generated_enemy =
                    new Mage(generated_props, kDefaultRangedEnemyStats);
                break;
            case 3:
                generated_enemy =
                    new Dog(generated_props, kDefaultRangedEnemyStats);
                break;
            case 4:
                generated_enemy =
                    new Skeleton(generated_props, kDefaultRangedEnemyStats);
                break;
            case 5:
                generated_enemy =
                    new Goblin(generated_props, kDefaultRangedEnemyStats);
                break;
            case 6:
                generated_enemy =
                    new HelixEnemy(generated_props, kDefaultRangedEnemyStats);
                break;
            default:
                break;
        }

        ColliderHandler::GetInstance()->AddCollider(generated_enemy);
        m_Objects.push_back(generated_enemy);
    }

    if (timer.GetTicks() % 5000 <= 10 &&
        max_tick_interval < cur_enemy_generation_interval) {
        SDL_Log("Increasing enemy generation interval");
        cur_enemy_generation_interval -= 200;
    }
}

void Game::Update(float dt) {
    ColliderHandler::GetInstance()->HandleCollisions();
    if (player != nullptr) {
        player->Update(dt);
        m_WeaponInventory->SetSelectedWeapon(player->GetCurrentWeapon());
        if (player->MarkedForDeletion()) {
            DeleteObject(player);
            player = nullptr;
            m_GameEventManager->SetPlayer(nullptr);
        }
    }
    for (auto it = m_Objects.begin(); it != m_Objects.end();) {
        auto* enemy = dynamic_cast<Enemy*>(*it);  // Cast to Enemy type
        if (enemy != nullptr) {
            enemy->SetTarget(player);
        }
        (*it)->Update(dt);
        if ((*it)->MarkedForDeletion()) {
            DeleteObject(*it);
        } else {
            ++it;
        }
    }

    //GenerateRandomEnemyIfNeeded();
}

void Game::Render() {
    Renderer::GetInstance()->RenderClear();
    for (auto* obj : m_Objects) {
        obj->Draw();
    }
    if (player != nullptr) {
        player->Draw();
        m_WeaponInventory->Draw();
        m_HeadsUpDisplay->Draw();
    }
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

Game::~Game() {
    for (auto* obj : m_Objects) {
        obj->Clean();
        delete obj;
    }
    m_Objects.clear();
    delete m_WeaponInventory;
    delete m_GameEventManager;
    delete m_HeadsUpDisplay;
    SDL_FreeCursor(SDL_GetCursor());
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif
