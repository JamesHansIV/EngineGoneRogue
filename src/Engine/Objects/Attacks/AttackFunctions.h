#include "Attack.h"
#include "Engine/Objects/Projectiles/Projectile.h"

std::vector<Projectile*> CreateBullet(RangedAttackInfo info);
std::vector<Projectile*> CreateSpreadBullet(RangedAttackInfo info);
std::vector<Projectile*> CreateHelixBullets(RangedAttackInfo info);
std::vector<Projectile*> CreateRotatingBullets(RangedAttackInfo info);
std::vector<Projectile*> CreateSpreadBullets(RangedAttackInfo info);
std::vector<Projectile*> CreateRingBullets(RangedAttackInfo info);
