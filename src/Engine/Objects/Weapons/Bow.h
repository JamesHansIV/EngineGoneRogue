#pragma once

#include "RangedWeapon.h"

class Bow : public RangedWeapon {
   public:
    Bow(Properties& props, RangedWeaponStats& stats, Player* owner)
        : RangedWeapon(props, stats, owner) {
        const int k_projectile_width = 10;
        const int k_projectile_height = 10;

        m_projectile_props =
            new Properties("arrow", {0, 0, 16, 14},
                           {GetMidPointX() * 2, GetMidPointY() * 2,
                            k_projectile_width * 2, k_projectile_height * 2},
                           GetRotation(), "arrow");
    }
};
