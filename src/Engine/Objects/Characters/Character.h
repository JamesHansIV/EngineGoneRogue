#pragma once
#include <string>
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"

class Character : public Collider {

   public:
    explicit Character(Properties& props) : Collider(props) {}

    Character(Collider* rhs) : Collider(rhs) {}

    void Draw() override = 0;
    void Clean() override = 0;
    virtual void Update(float dt) override = 0;

    Health* GetHealth() { return m_health; }

    void SetHealth(Health* health) { m_health = health; }

   protected:
    Health* m_health{};
    std::string m_name;
};
