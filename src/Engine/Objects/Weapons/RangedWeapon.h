#pragma once

#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Projectiles/ProjectileManager.h"
#include "Weapon.h"

class PlayerStats;
class Player;

class RangedWeaponStats : public WeaponStats {
   public:
    RangedWeaponStats(bool player_owned, Uint32 fire_rate,
                      Uint32 projectile_speed, int damage,
                      PlayerStats* ownerStats)
        : WeaponStats(player_owned, fire_rate, projectile_speed, damage,
                      ownerStats) {}

    RangedWeaponStats(const RangedWeaponStats& prop) = default;

    RangedWeaponStats& operator=(const RangedWeaponStats& prop) = default;

    ~RangedWeaponStats() = default;
};

class RangedWeapon : public Weapon {
   public:
    RangedWeapon(Properties& props, RangedWeaponStats& stats, Player* owner,
                 const std::string& name, Properties* projectile_props);
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    ObjectType GetObjectType() override { return ObjectType::RangedWeapon; }

    RangedWeaponStats* GetStats() override { return &m_stats; };

    ~RangedWeapon() override;

    Projectile* BuildProjectile();

   protected:
    ProjectileManager m_projectile_manager;
    bool m_auto_fire_enabled = false;
    RangedWeaponStats m_stats;
    Uint32 m_last_fired = 0;
    Properties* m_projectile_props = nullptr;
};
