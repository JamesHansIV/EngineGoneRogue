#pragma once
#include <string>
#include "Engine/Objects/Collider.h"
#include "GameObject.h"

class Character : public Collider {

   public:
    explicit Character(Properties& props)
        : Collider(props), m_Health(nullptr) {}

    void Draw() override = 0;
    void Clean() override = 0;
    virtual void Update(float dt) override = 0;

    Health* GetHealth() { return m_Health; }

    void SetHealth(Health* health) { m_Health = health; }

   protected:
    Health* m_Health;
    std::string m_Name;
};
