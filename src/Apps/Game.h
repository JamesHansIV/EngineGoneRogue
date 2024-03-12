#pragma once

#include <tinyxml2.h>
#include <iostream>
#include "Engine/Application/Application.h"
#include "Engine/Audio/AudioManager.h"
#include "Engine/Events/GameEventManager.h"
#include "Engine/Events/ItemManager.h"
#include "Engine/HeadsUpDisplay/HeadsUpDisplay.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/WeaponInventory.h"

class Game : public Application {
   public:
    Game();

    void Events() override;
    void Update(float dt) override;
    void Render() override;

    void AddObject(GameObject* obj);
    void DeleteObject(GameObject* obj);

    void UpdateObjects(float dt);
    void DrawObjects();

    void ChangeState(State* state);

    std::vector<GameObject*>::iterator GetObjects() {
        return m_objects.begin();
    }

    ~Game() override;

   private:
    void InitObjects();
    AudioManager m_audio_manager;
    State* m_state;
    std::vector<GameObject*> m_objects;
    WeaponInventory* m_weapon_inventory;
    int m_tick = 0;
    Uint32 m_last_enemy_spawn_time = 0;
    GameEventManager* m_game_event_manager;
    HUD* m_heads_up_display;
    ItemManager* m_item_manager;
    void GenerateRandomEnemyIfNeeded();
};
