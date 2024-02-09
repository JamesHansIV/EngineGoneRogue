#pragma once

#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"

class HealthPotion : public Collider {
   public:
    explicit HealthPotion(Properties& props, int health );
    void Draw() override { GameObject::Draw(); }

    void Clean() override {};

    ObjectType GetObjectType() override { return ObjectType::HealthPotion; }
    
    void Update(float dt) override;

    int GetHealth()  { return m_health; };

    void OnCollide(Collider* collidee) override;
    
   private:
    int m_health;
};