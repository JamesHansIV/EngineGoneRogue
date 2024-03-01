#include "Game.h"
#include <cstdlib>
#include <vector>
#include "Engine/Events/ItemManager.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Characters/Dog.h"
#include "Engine/Objects/Characters/EnemyStats.h"
#include "Engine/Objects/Characters/Goblin.h"
#include "Engine/Objects/Characters/HelixEnemy.h"
#include "Engine/Objects/Characters/Mage.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Characters/RingShotEnemy.h"
#include "Engine/Objects/Characters/Skeleton.h"
#include "Engine/Objects/Characters/Slime.h"
#include "Engine/Objects/Chests/Chest.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/HealthPotion.h"
#include "Engine/Objects/Item.h"
#include "Engine/Objects/WeaponInventory.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/Timer.h"
#include "SDL2/SDL_log.h"

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

Slime* SLIME_COPY = nullptr;
RingShotEnemy* RING_SHOT_ENEMY_COPY = nullptr;
Mage* MAGE_COPY = nullptr;
Dog* DOG_COPY = nullptr;
Skeleton* SKELETON_COPY = nullptr;
Goblin* GOBLIN_COPY = nullptr;
HelixEnemy* HELIX_ENEMY_COPY = nullptr;

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    m_Objects = Application::m_Rooms[m_BaseRoomID];

    for (auto& obj : m_Objects) {
        if (auto* collider = dynamic_cast<Collider*>(obj)) {
            EnemyStats enemy_stats;
            RangedEnemyStats ranged_enemy_stats;

            if (Enemy* enemy = dynamic_cast<Enemy*>(obj)) {
                enemy_stats = enemy->GetEnemyStats();
            } else {
                continue;
            }

            if (Slime* slime = dynamic_cast<Slime*>(obj)) {
                SLIME_COPY = new Slime(slime, enemy_stats);
            }

            if (RangedEnemy* ranged_enemy = dynamic_cast<RangedEnemy*>(obj)) {
                ranged_enemy_stats = ranged_enemy->GetRangedEnemyStats();
            } else {
                continue;
            }

            if (RingShotEnemy* ring_shot_enemy =
                    dynamic_cast<RingShotEnemy*>(obj)) {
                RING_SHOT_ENEMY_COPY =
                    new RingShotEnemy(ring_shot_enemy, ranged_enemy_stats);
            }
            if (Mage* mage = dynamic_cast<Mage*>(obj)) {
                MAGE_COPY = new Mage(mage, ranged_enemy_stats);
            }
            if (Dog* dog = dynamic_cast<Dog*>(obj)) {
                DOG_COPY = new Dog(dog, ranged_enemy_stats);
            }
            if (Skeleton* skeleton = dynamic_cast<Skeleton*>(obj)) {
                SKELETON_COPY = new Skeleton(skeleton, ranged_enemy_stats);
            }
            if (Goblin* goblin = dynamic_cast<Goblin*>(obj)) {
                GOBLIN_COPY = new Goblin(goblin, ranged_enemy_stats);
            }
            if (HelixEnemy* helix_enemy = dynamic_cast<HelixEnemy*>(obj)) {
                HELIX_ENEMY_COPY =
                    new HelixEnemy(helix_enemy, ranged_enemy_stats);
            }
            ColliderHandler::GetInstance()->AddCollider(collider);
        }
    }

    // specific stats replaced with defaults for easy refactor. can be changed later
    // specific enemy stats changed in commit on 2/4.

    Properties props11("healthpotion", {1, 1, 16, 16}, {250, 150, 25, 25}, 0,
                       "healthpotion");
    auto* healthpotion = new HealthPotion(props11, 20);

    Properties props12("healthpotion", {1, 1, 16, 16}, {550, 400, 25, 25}, 0,
                       "healthpotion2");
    auto* healthpotion2 = new HealthPotion(props12, 20);

    m_Objects.push_back(healthpotion);
    m_Objects.push_back(healthpotion2);
    ColliderHandler::GetInstance()->AddCollider(m_Player);
    ColliderHandler::GetInstance()->AddCollider(healthpotion);
    ColliderHandler::GetInstance()->AddCollider(healthpotion2);

    srand(time(nullptr));

    m_WeaponInventory = new WeaponInventory(m_Player->GetPlayerWeapons());
    m_WeaponInventory->SetSelectedWeapon(m_Player->GetCurrentWeapon());

    Renderer::GetInstance()->SetCameraTarget(m_Player);
    m_GameEventManager = new GameEventManager(m_Player, m_Objects);

    m_HeadsUpDisplay = new HUD(*m_Player);

    m_ItemManager = new ItemManager(m_Objects, m_Player);

    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    SDL_SetCursor(cursor);
}

void Game::Events() {
    m_GameEventManager->HandleEvents(m_ItemManager);
}

void Game::GenerateRandomEnemyIfNeeded() {
    // We probably want a wave multipler. Idea of waves.
    // Have dedicated stats for each enemy
    // Have a wave manager that will spaw100enemies
    // have a wave multiplier that will increase the stats of the enemies
    // Auto generate enemies
    if ((SDL_GetTicks() - m_last_enemy_spawn_time) >=
        cur_enemy_generation_interval) {
        m_last_enemy_spawn_time = SDL_GetTicks();
        // Generate random x and y coordinates
        float const generated_x = rand() % 500 + 200;
        float const generated_y = rand() % 300 + 20;

        // Generate random enemy type
        int const enemy_type = rand() % 7;
        Enemy* generated_enemy = nullptr;

        switch (enemy_type) {
            case 0:
                SLIME_COPY->SetX(generated_x);
                SLIME_COPY->SetY(generated_y);
                generated_enemy =
                    new Slime(SLIME_COPY, SLIME_COPY->GetEnemyStats());
                break;
            case 1:
                RING_SHOT_ENEMY_COPY->SetX(generated_x);
                RING_SHOT_ENEMY_COPY->SetY(generated_y);
                generated_enemy = new RingShotEnemy(
                    RING_SHOT_ENEMY_COPY,
                    RING_SHOT_ENEMY_COPY->GetRangedEnemyStats());
                break;
            case 2:
                MAGE_COPY->SetX(generated_x);
                MAGE_COPY->SetY(generated_y);
                generated_enemy =
                    new Mage(MAGE_COPY, MAGE_COPY->GetRangedEnemyStats());
                break;
            case 3:
                DOG_COPY->SetX(generated_x);
                DOG_COPY->SetY(generated_y);
                generated_enemy =
                    new Dog(DOG_COPY, DOG_COPY->GetRangedEnemyStats());
                break;
            case 4:
                SKELETON_COPY->SetX(generated_x);
                SKELETON_COPY->SetY(generated_y);
                generated_enemy = new Skeleton(
                    SKELETON_COPY, SKELETON_COPY->GetRangedEnemyStats());
                break;
            case 5:
                GOBLIN_COPY->SetX(generated_x);
                GOBLIN_COPY->SetY(generated_y);
                generated_enemy =
                    new Goblin(GOBLIN_COPY, GOBLIN_COPY->GetRangedEnemyStats());
                break;
            case 6:
                HELIX_ENEMY_COPY->SetX(generated_x);
                HELIX_ENEMY_COPY->SetY(generated_y);
                generated_enemy = new HelixEnemy(
                    HELIX_ENEMY_COPY, HELIX_ENEMY_COPY->GetRangedEnemyStats());
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
    ProjectileManager::GetInstance()->UpdateProjectiles(dt);
    if (m_Player != nullptr) {
        m_Player->Update(dt);
        m_HeadsUpDisplay->Update(*m_Player);
        if (m_Player->MarkedForDeletion()) {
            DeleteObject(m_Player);
            m_Player = nullptr;
            m_GameEventManager->SetPlayer(nullptr);
        }
    }
    for (auto it = m_Objects.begin(); it != m_Objects.end();) {
        auto* enemy = dynamic_cast<Enemy*>(*it);  // Cast to Enemy type
        if (enemy != nullptr) {
            enemy->SetTarget(m_Player);
        }
        (*it)->Update(dt);
        if ((*it)->MarkedForDeletion()) {
            DeleteObject(*it);
        } else {
            ++it;
        }
    }

    GenerateRandomEnemyIfNeeded();
}

void Game::Render() {
    Renderer::GetInstance()->RenderClear();

    for (auto* obj : m_Objects) {
        obj->Draw();
    }

    ProjectileManager::GetInstance()->Draw();
    if (m_Player != nullptr) {
        m_Player->Draw();
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
        SDL_Log("delete object: %s", (*it)->GetID().c_str());
        m_Objects.erase(it);
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
    delete ProjectileManager::GetInstance();
    SDL_FreeCursor(SDL_GetCursor());
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif
