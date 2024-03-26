#pragma once

#include "Engine/Objects/Collider.h"

enum BombState { ACTIVE, EXPLODING, EXPLODING_DAMAGING, INACTIVE };

struct GrenadeStats {
    int damage;
    int time_to_explode;
    int explosion_radius;
    int explosion_damage;
    int explosion_duration;
};

class Grenade : public Collider {
   public:
    explicit Grenade(Properties& props, GrenadeStats stats);

    void Draw() override { GameObject::Draw(); }

    void Clean() override{};

    ObjectType GetObjectType() override { return ObjectType::Grenade; }

    void Update(float dt) override;

    void OnCollide(Collider* collidee) override;

    [[nodiscard]] BombState GetState() const { return m_state; }

    [[nodiscard]] int GetDamage() const { return m_stats.damage; }

   private:
    GrenadeStats m_stats;
    double m_time_alive;
    BombState m_state;
};
