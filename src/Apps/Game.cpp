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
#include "Engine/State/GameState.h"
#include "Engine/Timer/Timer.h"
#include "Engine/UI/Button.h"
#include "SDL2/SDL_log.h"

std::vector<Collider*> colliders;

int max_tick_interval = 500;
int cur_enemy_generation_interval = 4000;
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

Slime* slime_copy = nullptr;
RingShotEnemy* ring_shot_enemy_copy = nullptr;
Mage* mage_copy = nullptr;
Dog* dog_copy = nullptr;
Skeleton* skeleton_copy = nullptr;
Goblin* goblin_copy = nullptr;
HelixEnemy* helix_enemy_copy = nullptr;

// Todo: try to clean this up
// https://stackoverflow.com/questions/27451776/dynamic-cast-for-multiple-derived-classes
void Game::InitObjects() {
    for (auto& obj : m_objects) {
        if (auto* collider = dynamic_cast<Collider*>(obj)) {
            EnemyStats enemy_stats;
            RangedEnemyStats ranged_enemy_stats;

            if (auto* enemy = dynamic_cast<Enemy*>(obj)) {
                enemy_stats = enemy->GetEnemyStats();
            } else {
                continue;
            }

            if (auto* slime = dynamic_cast<Slime*>(obj)) {
                slime_copy = new Slime(slime, enemy_stats);
            }

            if (auto* ranged_enemy = dynamic_cast<RangedEnemy*>(obj)) {
                ranged_enemy_stats = ranged_enemy->GetRangedEnemyStats();
            }

            if (auto* ring_shot_enemy = dynamic_cast<RingShotEnemy*>(obj)) {
                ring_shot_enemy_copy =
                    new RingShotEnemy(ring_shot_enemy, ranged_enemy_stats);
            }
            if (Mage* mage = dynamic_cast<Mage*>(obj)) {
                mage_copy = new Mage(mage, ranged_enemy_stats);
            }
            if (Dog* dog = dynamic_cast<Dog*>(obj)) {
                dog_copy = new Dog(dog, ranged_enemy_stats);
            }
            if (auto* skeleton = dynamic_cast<Skeleton*>(obj)) {
                skeleton_copy = new Skeleton(skeleton, ranged_enemy_stats);
            }
            if (auto* goblin = dynamic_cast<Goblin*>(obj)) {
                goblin_copy = new Goblin(goblin, ranged_enemy_stats);
            }
            if (auto* helix_enemy = dynamic_cast<HelixEnemy*>(obj)) {
                helix_enemy_copy =
                    new HelixEnemy(helix_enemy, ranged_enemy_stats);
            }
        }
    }
}

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    m_objects = Application::m_rooms[m_base_room_id];

    Collider* c = nullptr;
    for (auto* obj : m_objects) {
        if ((c = dynamic_cast<Collider*>(obj)) != nullptr) {
            ColliderHandler::GetInstance()->AddCollider(c);
        }
    }

    InitObjects();

    Properties props11("healthpotion", {1, 1, 16, 16}, {250, 150, 25, 25}, 0,
                       "healthpotion");
    auto* healthpotion = new HealthPotion(props11, 20);

    Properties props12("healthpotion", {1, 1, 16, 16}, {550, 400, 25, 25}, 0,
                       "healthpotion2");
    auto* healthpotion2 = new HealthPotion(props12, 20);

    m_objects.push_back(healthpotion);
    m_objects.push_back(healthpotion2);
    ColliderHandler::GetInstance()->AddCollider(m_player);
    ColliderHandler::GetInstance()->AddCollider(healthpotion);
    ColliderHandler::GetInstance()->AddCollider(healthpotion2);

    srand(time(nullptr));

    m_weapon_inventory = new WeaponInventory(m_player->GetPlayerWeapons());
    m_weapon_inventory->SetSelectedWeapon(m_player->GetCurrentWeapon());

    Renderer::GetInstance()->SetCameraTarget(m_player);
    m_game_event_manager = new GameEventManager(m_player, m_objects);

    m_heads_up_display = new HUD(*m_player);

    m_item_manager = new ItemManager(m_objects, m_player);

    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);

    SDL_SetCursor(cursor);

    ChangeState(new StartState(this));
}

void Game::Events() {
    State* state = m_game_event_manager->HandleEvents(m_item_manager, m_state);
    if (state != nullptr) {
        ChangeState(state);
    }
}

void Game::GenerateRandomEnemyIfNeeded() {
    // We probably want a wave multipler. Idea of waves.
    // Have dedicated stats for each enemy
    // Have a wave manager that will spaw100enemies
    // have a wave multiplier that will increase the stats of the enemies
    // Auto generate enemies
    const Uint32 current_time = timer.GetTicks();
    if ((current_time - m_last_enemy_spawn_time) >=
        cur_enemy_generation_interval) {
        m_last_enemy_spawn_time = current_time;
        // Generate random x and y coordinates
        // Todo: make sure the x and y coordinates are not on top of the Player
        // and only spawn in current viewing area
        for (int i = 0; i < m_enemies_to_spawn; i++) {
            GenerateRandomEnemy();
        }

        m_enemy_stat_multiplier += 0.0025;
        m_wave_count += 1;

        if (m_wave_count % 5 == 0 &&
            max_tick_interval > cur_enemy_generation_interval) {
            SDL_Log("Increasing enemy generation interval");
            cur_enemy_generation_interval -= 100;
        }

        if (m_wave_count % 10 == 0) {
            m_enemies_to_spawn += 1;
        }
    }
}

void Game::GenerateRandomEnemy() {
    float const generated_x = rand() % 500 + 200;
    float const generated_y = rand() % 300 + 20;

    // Generate random enemy type
    int const enemy_type = rand() % 7;
    Enemy* generated_enemy = nullptr;

    switch (enemy_type) {
        case 0:
            slime_copy->SetX(generated_x);
            slime_copy->SetY(generated_y);
            generated_enemy =
                new Slime(slime_copy, slime_copy->GetEnemyStats() *
                                          m_enemy_stat_multiplier);
            break;
        case 1:
            ring_shot_enemy_copy->SetX(generated_x);
            ring_shot_enemy_copy->SetY(generated_y);
            generated_enemy =
                new RingShotEnemy(ring_shot_enemy_copy,
                                  ring_shot_enemy_copy->GetRangedEnemyStats() *
                                      m_enemy_stat_multiplier);
            break;
        case 2:
            mage_copy->SetX(generated_x);
            mage_copy->SetY(generated_y);
            generated_enemy =
                new Mage(mage_copy, mage_copy->GetRangedEnemyStats() *
                                        m_enemy_stat_multiplier);
            break;
        case 3:
            dog_copy->SetX(generated_x);
            dog_copy->SetY(generated_y);
            generated_enemy =
                new Dog(dog_copy, dog_copy->GetRangedEnemyStats() *
                                      m_enemy_stat_multiplier);
            break;
        case 4:
            skeleton_copy->SetX(generated_x);
            skeleton_copy->SetY(generated_y);
            generated_enemy = new Skeleton(
                skeleton_copy,
                skeleton_copy->GetRangedEnemyStats() * m_enemy_stat_multiplier);
            break;
        case 5:
            goblin_copy->SetX(generated_x);
            goblin_copy->SetY(generated_y);
            generated_enemy =
                new Goblin(goblin_copy, goblin_copy->GetRangedEnemyStats() *
                                            m_enemy_stat_multiplier);
            break;
        case 6:
            helix_enemy_copy->SetX(generated_x);
            helix_enemy_copy->SetY(generated_y);
            generated_enemy = new HelixEnemy(
                helix_enemy_copy, helix_enemy_copy->GetRangedEnemyStats() *
                                      m_enemy_stat_multiplier);
            break;
        default:
            break;
    }

    ColliderHandler::GetInstance()->AddCollider(generated_enemy);
    m_objects.push_back(generated_enemy);
}

void Game::Update(float dt) {
    m_state->Update(dt);
}

void Game::UpdateObjects(float dt) {
    // Note: not needed if GameState is in sync with timer
    if (timer.IsPaused()) {
        return;
    }
    ColliderHandler::GetInstance()->HandleCollisions();
    ProjectileManager::GetInstance()->UpdateProjectiles(dt);
    if (m_player != nullptr) {
        m_player->Update(dt);
        m_heads_up_display->Update(*m_player);
        if (m_player->MarkedForDeletion()) {
            DeleteObject(m_player);
            m_player = nullptr;
            m_game_event_manager->SetPlayer(nullptr);
            ChangeState(new StartState(this));
            return;
        }
    }
    for (auto it = m_objects.begin(); it != m_objects.end();) {
        auto* enemy = dynamic_cast<Enemy*>(*it);  // Cast to Enemy type
        if (enemy != nullptr) {
            enemy->SetTarget(m_player);
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
    m_state->Draw();
    Renderer::GetInstance()->Render();
}

void Game::DrawObjects() {
    for (auto* obj : m_objects) {
        obj->Draw();
    }

    ProjectileManager::GetInstance()->Draw();
    if (m_player != nullptr) {
        m_player->Draw();
        m_heads_up_display->Draw(*m_state);
    }
}

void Game::ChangeState(State* state) {
    if (m_state != nullptr) {
        m_state->Exit();
        delete m_state;
    }

    m_state = state;
    m_state->Enter();
}

void Game::AddObject(GameObject* obj) {
    SDL_Log("Adding object: %s", obj->GetID().c_str());
    m_objects.push_back(obj);
}

void Game::DeleteObject(GameObject* obj) {
    auto it = std::find(m_objects.begin(), m_objects.end(), obj);
    if (it != m_objects.end()) {
        SDL_Log("delete object: %s", (*it)->GetID().c_str());
        m_objects.erase(it);
    }
    obj->Clean();
    delete obj;
    obj = nullptr;
}

Game::~Game() {
    for (auto* obj : m_objects) {
        obj->Clean();
        delete obj;
    }

    delete slime_copy;
    delete dog_copy;
    delete ring_shot_enemy_copy;
    delete mage_copy;
    delete skeleton_copy;
    delete goblin_copy;
    delete helix_enemy_copy;
    delete m_weapon_inventory;
    delete m_game_event_manager;
    delete m_heads_up_display;
    delete ProjectileManager::GetInstance();
    SDL_FreeCursor(SDL_GetCursor());
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif
