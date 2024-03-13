#pragma once

#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Projectiles/Projectile.h"

class RotatingBullet : public Projectile {

   public:
    RotatingBullet(Properties& props, float speed, float angle,
                   bool playerOwned = false, float rotationAngle = 0.0F,
                   float rotationSpeed = M_PI / 64, float radius = 40);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override {
        return ObjectType::Projectile;
    }

    ProjectileType GetProjectileType() override {
        return ProjectileType::Rotating;
    }

   private:
    Vector2D m_center;
    float m_rotation_speed;
    float m_rotation_angle;
    float m_radius;
};
