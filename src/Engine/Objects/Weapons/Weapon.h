#pragma once
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Projectiles/Projectile.h"

class PlayerStats;
class Player;

// Potential stats:
// Damage, Accuracy, Pattern?, spread?, range, knockback, elemental, status effect, armor penetration, reload, switching speed,

class WeaponStats {
   public:
    explicit WeaponStats(bool playerOwned = false, Uint32 fireRate = 200,
                         Uint32 projectileSpeed = 10, int damage = 10,
                         PlayerStats* ownerStats = nullptr) {
        m_player_owned = playerOwned;
        m_fire_rate = fireRate;
        m_projectile_speed = projectileSpeed;
        m_damage = damage;
        m_ownerStats = ownerStats;
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

    [[nodiscard]] int GetDamage() const { return m_damage; }

    void SetDamage(int damage) { m_damage = damage; }

    PlayerStats* GetOwnerStats() { return m_ownerStats; };

   protected:
    bool m_player_owned;
    Uint32 m_fire_rate;
    Uint32 m_projectile_speed;
    int m_damage;
    PlayerStats* m_ownerStats;
};

class Weapon : public Collider {
   public:
    explicit Weapon(Properties& props, WeaponStats& stats, Player* owner)
        : Collider(props) {
        m_stats = stats;
        m_owner = owner;
    }

    void Draw() override { GameObject::Draw(); }

    void Clean() override = 0;
    void Update(float dt) override = 0;

    WeaponStats GetStats() { return m_stats; };

    inline bool IsPlayerOwned() { return m_stats.IsPlayerOwned(); }

    Player* GetOwner() { return m_owner; };

   private:
    WeaponStats m_stats;
    bool m_auto_fire_enabled = false;
    Player* m_owner;
};
