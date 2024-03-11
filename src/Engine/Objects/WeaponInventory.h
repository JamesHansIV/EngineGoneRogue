#pragma once
#include <vector>
#include "Weapons/Weapon.h"

class WeaponInventory {
   public:
    WeaponInventory(std::vector<Weapon*> weapons)
        : m_weapons(std::move(weapons)){};

    void Draw();

    void Clean();

    void Update(const Player& player);

    void SetSelectedWeapon(Weapon* weapon) { m_selected_weapon = weapon; };

   private:
    std::vector<Weapon*> m_weapons;
    Weapon* m_selected_weapon;
};
