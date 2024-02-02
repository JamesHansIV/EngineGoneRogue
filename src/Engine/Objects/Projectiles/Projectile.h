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
               bool playerOwned = false, int damage = 10);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void CheckOutOfBounds();

    void OnCollide(Collider* collidee) override;

    [[nodiscard]] bool IsPlayerOwned() const { return m_PlayerOwned; }

    [[nodiscard]] float GetSpeed() const { return m_Speed; }

    void SetSpeed(float speed) { m_Speed = speed; }

    [[nodiscard]] float GetAngle() const { return m_Angle; }

    void SetAngle(float angle) { m_Angle = angle; }

    Vector2D GetVelocity() { return m_Velocity; }

    void SetVelocity(Vector2D velocity) { m_Velocity = velocity; }

    [[nodiscard]] int GetDamage() const { return m_Damage; }

    void SetDamage(int damage) { m_Damage = damage; }

    ObjectType GetObjectType() override { return ObjectType::Projectile; }

    virtual ProjectileType GetProjectileType() {
        return ProjectileType::Standard;
    }

   private:
    bool m_PlayerOwned;
    float m_Speed;
    float m_Angle;
    Vector2D m_Velocity;
    int m_Damage;
};
