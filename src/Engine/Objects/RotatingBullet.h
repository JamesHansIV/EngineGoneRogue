#pragma once

#include "Engine/Objects/Projectile.h"
#include "GameObject.h"

class RotatingBullet : public Projectile {

   public:
    RotatingBullet(Properties& props, float speed, float angle,
                   bool playerOwned = false, float rotationAngle = 0.0f,
                   float rotationSpeed = M_PI / 36, float radius = 40);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override {
        return ObjectType::Projectile;
    }

   private:
    Vector2D m_Center;
    float m_RotationSpeed;
    float m_RotationAngle;
    float m_Radius;
};
