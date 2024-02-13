#pragma once
#include <vector>
#include "Weapons/Weapon.h"

class WeaponInventory{
   public:
    WeaponInventory(std::vector<Weapon*> weapons) : m_Weapons(std::move(weapons)) {};

    void Draw();

    void Clean();
    
    void Update(float dt);

    void SetSelectedWeapon (Weapon* weapon) {m_SelectedWeapon = weapon; };
    
   private:
    std::vector<Weapon*> m_Weapons;
    Weapon* m_SelectedWeapon;
};