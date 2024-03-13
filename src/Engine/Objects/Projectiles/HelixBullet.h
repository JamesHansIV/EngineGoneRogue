#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Physics/RigidBody.h"
#include "Projectile.h"
#include "SDL2/SDL.h"
#include "functional"

#define PI4 (M_PI / 4)

class HelixBullet : public Projectile {
   public:
    HelixBullet(Properties& props, float speed, float angle,
                bool playerOwned = false, bool flipped = false);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override {
        return ObjectType::Projectile;
    }

    ProjectileType GetProjectileType() override {
        return ProjectileType::Helix;
    }

   private:
    Vector2D m_origin;
    Vector2D m_t_pos;
    float m_wave_magnitude{1};
    float m_scale{30.0F};
    int m_flipped;
};
