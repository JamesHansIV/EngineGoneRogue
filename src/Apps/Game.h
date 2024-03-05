#pragma once

#include <tinyxml2.h>
#include <iostream>
#include "Engine/Application/Application.h"
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
        return m_Objects.begin();
    }

    ~Game();

   private:
    State* m_State;
    std::vector<GameObject*> m_Objects;
    WeaponInventory* m_WeaponInventory;
    int m_tick = 0;
    Uint32 m_last_enemy_spawn_time = 0;
    GameEventManager* m_GameEventManager;
    HUD* m_HeadsUpDisplay;
    ItemManager* m_ItemManager;
    void GenerateRandomEnemyIfNeeded();
};
