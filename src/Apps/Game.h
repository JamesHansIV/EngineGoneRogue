#pragma once

#include <tinyxml2.h>
#include <iostream>
#include "Engine/Application/Application.h"
#include "Engine/Audio/AudioManager.h"
#include "Engine/Events/GameEventManager.h"
#include "Engine/Events/ItemManager.h"
#include "Engine/HeadsUpDisplay/HeadsUpDisplay.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/UI/WeaponInventory.h"

class Game : public Application {
   public:
    Game();

    void Events() override;
    void Update(float dt) override;
    void Render() override;

    void Restart();

    void InitManagers();

    void AddObject(GameObject* obj);
    void DeleteObject(GameObject* obj);

    void HandleEvent(RoomTransitionEvent* event);

    void ResetManagers();

    void UpdateObjects(float dt);
    void DrawObjects();

    void ChangeState(State* state);

    ~Game() override;

   private:
    std::vector<GameObject*> CopyObjects(
        const std::vector<GameObject*>& objects);
    void InitEnemyCopies();
    ItemInventory m_item_inventory;

    State* m_state;
    WeaponInventory* m_weapon_inventory;
    int m_tick = 0;
    Uint32 m_last_enemy_spawn_time = 0;
    GameEventManager* m_game_event_manager;
    HUD* m_heads_up_display;
    ItemManager* m_item_manager;
    float m_enemy_stat_multiplier = 1.0;
    Uint32 m_enemies_to_spawn = 1;
    Uint32 m_wave_count = 0;
    void GenerateRandomEnemyIfNeeded();
    void GenerateRandomEnemy();
};
