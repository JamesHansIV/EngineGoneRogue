#include "Game.h"
#include <cstdlib>
#include <string>
#include <vector>
#include "Engine/Events/ItemManager.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Characters/Charger.h"
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
#include "Engine/Objects/Trap.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/State/GameState.h"
#include "Engine/Timer/Timer.h"
#include "Engine/UI/Button.h"
#include "Engine/UI/WeaponInventory.h"
#include "Engine/utils/utils.h"
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
Charger* charger_copy = nullptr;

std::vector<GameObject*> Game::CopyObjects(
    const std::vector<GameObject*>& objects) {
    std::vector<GameObject*> new_objects;
    for (auto* obj : objects) {
        new_objects.push_back(obj->Copy());
        new_objects.back()->SetID(std::to_string(++object_counter));
    }
    return new_objects;
}

// Todo: try to clean this up
// https://stackoverflow.com/questions/27451776/dynamic-cast-for-multiple-derived-classes
void Game::InitEnemyCopies() {
    Enemy* enemy = nullptr;
    RangedEnemy* ranged_enemy = nullptr;
    for (auto* obj : m_objects) {
        if ((enemy = dynamic_cast<Slime*>(obj)) != nullptr) {
            slime_copy = new Slime(enemy, enemy->GetEnemyStats());
            continue;
        }

        if ((enemy = dynamic_cast<Charger*>(obj)) != nullptr) {
            charger_copy = new Charger(enemy, enemy->GetEnemyStats());
            continue;
        }

        if ((ranged_enemy = dynamic_cast<RingShotEnemy*>(obj)) != nullptr) {
            ring_shot_enemy_copy = new RingShotEnemy(
                ranged_enemy, ranged_enemy->GetRangedEnemyStats());
            continue;
        }
        if ((ranged_enemy = dynamic_cast<Mage*>(obj)) != nullptr) {
            mage_copy =
                new Mage(ranged_enemy, ranged_enemy->GetRangedEnemyStats());
            continue;
        }
        if ((ranged_enemy = dynamic_cast<Dog*>(obj)) != nullptr) {
            dog_copy =
                new Dog(ranged_enemy, ranged_enemy->GetRangedEnemyStats());
            continue;
        }
        if ((ranged_enemy = dynamic_cast<Skeleton*>(obj)) != nullptr) {
            skeleton_copy =
                new Skeleton(ranged_enemy, ranged_enemy->GetRangedEnemyStats());
            continue;
        }
        if ((ranged_enemy = dynamic_cast<Goblin*>(obj)) != nullptr) {
            goblin_copy =
                new Goblin(ranged_enemy, ranged_enemy->GetRangedEnemyStats());
            continue;
        }
        if ((ranged_enemy = dynamic_cast<HelixEnemy*>(obj)) != nullptr) {
            helix_enemy_copy = new HelixEnemy(
                ranged_enemy, ranged_enemy->GetRangedEnemyStats());
            continue;
        }
    }
}

Game::Game() : m_state(nullptr), m_endless(false) {
    srand(time(nullptr));

    InitManagers();

    InitEnemyCopies();

    Properties p_trap("", {0, 0, 16, 16}, {500, 500, 32, 32}, 0.0);
    Trap* trap = new Trap(p_trap, 30);
    m_objects.push_back(trap);
    Properties p_trap2("", {0, 0, 16, 16}, {600, 1000, 32, 32}, 0.0);
    Trap* trap2 = new Trap(p_trap2, 30);
    m_objects.push_back(trap2);
    Properties p_trap3("", {0, 0, 16, 16}, {1000, 500, 32, 32}, 0.0);
    Trap* trap3 = new Trap(p_trap3, 30);
    m_objects.push_back(trap3);

    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);

    SDL_SetCursor(cursor);

    m_weapon_inventory->SetSelectedWeapon(m_player->GetCurrentWeapon());

    State* state = new StartState(*this);

    ChangeState(state);
}

void Game::InitEndless() {
    GameObject* to_delete = nullptr;
    for (auto it = m_objects.begin(); it != m_objects.end();) {
        if (dynamic_cast<Entrance*>(*it) != nullptr) {
            to_delete = *it;
            it = m_objects.erase(it);
            delete to_delete;
            to_delete = nullptr;
        } else {
            it++;
        }
    }
    InitEnemyCopies();
}

void Game::InitManagers() {
    m_weapon_inventory = new WeaponInventory(GetPlayer()->GetPlayerWeapons());
    m_weapon_inventory->SetSelectedWeapon(GetPlayer()->GetCurrentWeapon());

    m_game_event_manager = new GameEventManager(GetPlayer());

    m_heads_up_display = new HUD(*GetPlayer());

    m_item_manager = new ItemManager(GetPlayer());
}

void Game::ResetManagers() {
    m_weapon_inventory->SetWeapons(m_player->GetPlayerWeapons());
    m_weapon_inventory->SetSelectedWeapon(GetPlayer()->GetCurrentWeapon());
    m_heads_up_display->Reset(*GetPlayer());
}

void Game::Restart() {
    GetPlayer()->Clean();
    GetPlayer()->Init();
    ResetManagers();
    SetStartPosition(m_global_start.first, m_global_start.second);
    LoadRoom(GetStartID());
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
    float const generated_x = rand() % 1600 + 200;
    float const generated_y = rand() % 1800 + 20;

    // Generate random enemy type
    int const enemy_type = rand() % 8;
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
        case 7:
            charger_copy->SetX(generated_x);
            charger_copy->SetY(generated_y);
            generated_enemy =
                new Charger(charger_copy, charger_copy->GetEnemyStats() *
                                              m_enemy_stat_multiplier);
            break;
        default:
            break;
    }

    generated_enemy->SetID(std::to_string(++object_counter));
    ColliderHandler::GetInstance()->AddCollider(generated_enemy);
    m_objects.push_back(generated_enemy);
}

void Game::HandleEvent(RoomTransitionEvent* event) {
    ChangeState(new RoomTransitionState(*this, event->GetNextRoomID()));
}

void Game::HandleEvent(StartGameEvent* event) {
    SDL_Log("in handle event for start game event");
    if (m_endless) {
        InitEndless();
    }
    ChangeState(new RunningState(*this));
}

void Game::Update(float dt) {
    State* state = m_state->Update(dt);
    if (state != nullptr) {
        ChangeState(state);
    }
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
            PushNewEvent(EventType::GameOverEvent);
            return;
        }

        ColliderHandler::GetInstance()->AddCollider(m_player);
    }

    for (auto* tile : m_tiles) {
        auto* t = dynamic_cast<Collider*>(tile);
        if (t != nullptr) {
            ColliderHandler::GetInstance()->AddCollider(t);
        }
    }

    for (int i = 0; i < m_objects.size(); i++) {  // NOLINT: need index deletion
        auto* object = m_objects[i];
        auto* enemy = dynamic_cast<Enemy*>(object);  // Cast to Enemy type
        if (enemy != nullptr) {
            enemy->SetTarget(m_player);
        }
        (object)->Update(dt);

        if ((object)->MarkedForDeletion()) {
            DeleteObject(object);
        } else {
            ColliderHandler::GetInstance()->AddCollider(
                dynamic_cast<Collider*>(object));
            ++object;
        }
    }

    if (m_endless) {
        GenerateRandomEnemyIfNeeded();
    }
}

void Game::Render() {
    Renderer::GetInstance().RenderClear();
    m_state->Draw();
    Renderer::GetInstance().Render();
}

void Game::DrawObjects() {
    for (auto* tile : m_tiles) {
        tile->Draw();
    }
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
    delete m_weapon_inventory;
    delete m_game_event_manager;
    delete m_heads_up_display;
    delete m_item_manager;
    delete slime_copy;
    delete dog_copy;
    delete ring_shot_enemy_copy;
    delete mage_copy;
    delete skeleton_copy;
    delete goblin_copy;
    delete helix_enemy_copy;
    delete m_state;
    delete ProjectileManager::GetInstance();
    SDL_FreeCursor(SDL_GetCursor());
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif
