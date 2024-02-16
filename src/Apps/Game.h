#pragma once

#include <tinyxml2.h>
#include <iostream>
#include "Engine/Application/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/GameEventManager.h"
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

    void ChestDrops(float chest_x, float chest_y);

    ~Game();

   private:
    std::vector<GameObject*> m_Objects;
    WeaponInventory* m_WeaponInventory;
    int m_tick = 0;
    Uint32 m_last_enemy_spawn_time = 0;
    GameEventManager* m_GameEventManager;
    float m_ChanceOfDrop = 0.1;

    void GenerateRandomEnemyIfNeeded();
};
