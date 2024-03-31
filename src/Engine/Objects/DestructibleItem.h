#pragma once

#include "Engine/Objects/Collider.h"

class DestructibleItem : public Collider {
   public:
    explicit DestructibleItem(Properties& props);
    explicit DestructibleItem(Collider* collider);

    void Draw() override { GameObject::Draw(); }

    void Clean() override{};

    ObjectType GetObjectType() override { return ObjectType::DestructibleItem; }

    void Update(float dt) override;

    void OnCollide(Collider* collidee) override;

   private:
};