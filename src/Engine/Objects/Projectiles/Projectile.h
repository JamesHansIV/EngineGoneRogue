#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "functional"

class Player;

enum class ProjectileType { Standard, Rotating, Helix };

class Projectile : public Collider {

   public:
    bool m_Active = true;
    Projectile(Properties& props, float speed, float angle,
               AnimationInfo m_hitAnimationInfo, bool playerOwned = false,
               int damage = 10, int piercing = 0, Player* owner = nullptr);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void CheckOutOfBounds();

    //Called when an opposing character is hit
    void HitTarget();

    bool Hit() { return m_hit; }

    void OnCollide(Collider* collidee) override;

    [[nodiscard]] bool IsPlayerOwned() const { return m_player_owned; }

    [[nodiscard]] float GetSpeed() const { return m_speed; }

    void SetSpeed(float speed) { m_speed = speed; }

    [[nodiscard]] float GetAngle() const { return m_angle; }

    void SetAngle(float angle) { m_angle = angle; }

    Vector2D GetVelocity() { return m_velocity; }

    void SetVelocity(Vector2D velocity) { m_velocity = velocity; }

    [[nodiscard]] int GetDamage() const { return m_damage; }

    void SetDamage(int damage) { m_damage = damage; }

    ObjectType GetObjectType() override { return ObjectType::Projectile; }

    virtual ProjectileType GetProjectileType() {
        return ProjectileType::Standard;
    }

    void SetPiercing(int piercing) { m_piercing = piercing; }

    [[nodiscard]] int GetPiercing() const { return m_piercing; }

    void AddNumberofEnemiesHit() { m_numberof_enemies_hit++; }

    [[nodiscard]] int GetNumberofEnemiesHit() const {
        return m_numberof_enemies_hit;
    };

    Player* GetOwner() { return m_owner; };

   private:
    bool m_player_owned;
    bool m_hit;
    AnimationInfo m_hit_animation_info;
    float m_speed;
    float m_angle;
    Vector2D m_velocity;
    int m_damage;
    int m_piercing;
    int m_numberof_enemies_hit = 0;
    Player* m_owner;
};
