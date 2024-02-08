#pragma once

#include "Weapon.h"

class MeleeWeaponStats : public WeaponStats {
   public:
    MeleeWeaponStats(bool playerOwned, Uint32 fireRate, Uint32 projectileSpeed,
                     int damage, PlayerStats* ownerStats)
        : WeaponStats(playerOwned, fireRate, projectileSpeed, damage,
                      ownerStats) {}

    MeleeWeaponStats(const MeleeWeaponStats& prop) = default;

    MeleeWeaponStats& operator=(const MeleeWeaponStats& prop) = default;

    ~MeleeWeaponStats() = default;
};

class MeleeWeapon : public Weapon {
   public:
    MeleeWeapon(Properties& props, MeleeWeaponStats& stats, Player* owner);

    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::MeleeWeapon; }

    ~MeleeWeapon() override = default;

   private:
    MeleeWeaponStats m_stats;
    Uint32 m_duration = 100;
    Uint32 m_LastAttackTime = 0;
};
