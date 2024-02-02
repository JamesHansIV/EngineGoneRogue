#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "functional"

enum class ProjectileType { Standard, Rotating, Helix };

class Projectile : public Collider {

   public:
    Projectile(Properties& props, float speed, float angle,
               bool playerOwned = false);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    void CheckOutOfBounds();

    virtual void OnCollide(Collider* collidee) override;

    bool PlayerOwned() { return m_PlayerOwned; }

    float GetSpeed() { return m_Speed; }

    void SetSpeed(float speed) { m_Speed = speed; }

    float GetAngle() { return m_Angle; }

    void SetAngle(float angle) { m_Angle = angle; }

    Vector2D GetVelocity() { return m_Velocity; }

    void SetVelocity(Vector2D velocity) { m_Velocity = velocity; }

    virtual ObjectType GetObjectType() override {
        return ObjectType::Projectile;
    }

    virtual ProjectileType GetProjectileType() {
        return ProjectileType::Standard;
    }

   private:
    bool m_PlayerOwned;
    float m_Speed;
    float m_Angle;
    Vector2D m_Velocity;
};
