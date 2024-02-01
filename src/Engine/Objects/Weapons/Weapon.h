#pragma once
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Projectiles/Projectile.h"

// Potential stats:
// Damage, Accuracy, Pattern?, spread?, range, knockback, elemental, status effect, armor penetration, reload, switching speed,

class WeaponStats {
   public:
    explicit WeaponStats(bool playerOwned = false, Uint32 fireRate = 200,
                         Uint32 projectileSpeed = 10) {
        m_player_owned = playerOwned;
        m_fire_rate = fireRate;
        m_projectile_speed = projectileSpeed;
    }

    WeaponStats(const WeaponStats& prop) = default;

    WeaponStats& operator=(const WeaponStats& prop) = default;

    ~WeaponStats() = default;

    [[nodiscard]] bool IsPlayerOwned() const { return m_player_owned; }

    [[nodiscard]] Uint32 GetFireRate() const { return m_fire_rate; }

    void SetFireRate(Uint32 fireRate) { m_fire_rate = fireRate; }

    [[nodiscard]] Uint32 GetProjectileSpeed() const {
        return m_projectile_speed;
    }

    void SetProjectileSpeed(Uint32 projectileSpeed) {
        m_projectile_speed = projectileSpeed;
    }

   protected:
    bool m_player_owned;
    Uint32 m_fire_rate;
    Uint32 m_projectile_speed;
};

class Weapon : public Collider {
   public:
    explicit Weapon(Properties& props, WeaponStats& stats) : Collider(props) {
        m_stats = stats;
    }

    void Draw() override { GameObject::Draw(); }

    virtual void Clean() override = 0;
    virtual void Update(float dt) override = 0;

    WeaponStats GetStats() { return m_stats; };

    inline bool PlayerOwned() { return m_stats.IsPlayerOwned(); }

   private:
    WeaponStats m_stats;
    bool m_auto_fire_enabled = false;
};
