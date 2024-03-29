#pragma once

#include "Engine/Objects/Attacks/AttackFunctions.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Weapons/RangedWeapon.h"

class Shotgun : public RangedWeapon {
   public:
    Shotgun(Properties& props, RangedWeaponStats& stats, Player* owner,
            const std::string& name, Properties* projectile_props)
        : RangedWeapon(props, stats, owner, name, projectile_props) {
        m_attack =
            RangedAttack(CreateShotgunSpreadBullets, m_stats.GetFireRate());
    }

    void Draw() override {
        RangedWeapon::Draw();
        m_attack.Draw();
    }

    void Clean() override { RangedWeapon::Clean(); }

    void Update(float dt) override;

    ObjectType GetObjectType() override { return ObjectType::RangedWeapon; }

    ~Shotgun() override = default;

   private:
    RangedAttack m_attack;
};
