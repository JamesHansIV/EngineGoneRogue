#pragma once

#include "Engine/Objects/Collider.h"

enum TrapState {
    IDLE,
    TOUCHEDBYPLAYER,
    OPENING,
    OPENED,
    CLOSING,
};

class Trap : public Collider {
   public:
    Trap(Properties& props, int damage);

    Trap(Collider* collider, int damage);

    void Draw() override { GameObject::Draw(); }

    void Clean() override{};

    GameObject* Copy() override;

    ObjectType GetObjectType() override { return ObjectType::Trap; }

    void Update(float dt) override;

    void OnCollide(Collider* collidee) override;

    [[nodiscard]] TrapState GetState() const { return m_state; }

    void SetAnimationAndState(const std::string& animationName,
                              TrapState state);

    [[nodiscard]] int GetDamage() const { return m_damage; };

   private:
    TrapState m_state;
    int m_damage;
    bool m_colliding_with_player;
};
