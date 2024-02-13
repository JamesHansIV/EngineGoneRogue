#pragma once

#include <tinyxml2.h>
#include <iostream>
#include "Engine/Application/Application.h"
#include "Engine/Events/Event.h"
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

   private:
    std::vector<GameObject*> m_Objects;
    int m_tick;
    WeaponInventory* m_WeaponInventory;
};
