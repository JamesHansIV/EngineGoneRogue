#pragma once

#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"

class GrenadeDrop : public Collider {
   public:
    explicit GrenadeDrop(Properties& props) : Collider(props){
        m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
    };
    void Draw() override { GameObject::Draw(); }
    void OnCollide(Collider* collidee) override;
};