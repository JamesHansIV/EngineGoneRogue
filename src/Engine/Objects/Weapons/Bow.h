#pragma once

#include "RangedWeapon.h"

class Bow : public RangedWeapon {
   public:
    Bow(Properties& props, RangedWeaponStats& stats, Player* owner)
        : RangedWeapon(props, stats, owner) {
        const int k_projectile_width = 10;
        const int k_projectile_height = 10;

        // Todo: Hack as we are mallocing the projectile_props in the parent class
        // Probably should have a virtual function to create the projectile props
        delete m_projectile_props;
        m_projectile_props =
            new Properties("arrow", {0, 0, 16, 14},
                           {GetMidPointX() * 2, GetMidPointY() * 2,
                            k_projectile_width * 2, k_projectile_height * 2},
                           GetRotation(), "arrow");
    }
};
