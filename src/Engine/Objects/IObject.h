#pragma once

enum class ObjectType {
    None = 0,
    Base,
    Collider,
    Projectile,
    Player,
    Enemy,
    Weapon,
    RangedWeapon,
    MeleeWeapon,
    Entrance,
    HealthPotion,
    Chest
};

class IObject {
   public:
    virtual void Draw() = 0;
    virtual void Update(float dt) = 0;
    virtual void Clean() = 0;
};
