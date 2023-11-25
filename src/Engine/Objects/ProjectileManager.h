#include "Engine/Objects/Projectile.h"

class ProjectileManager {
    public:
        void AddProjectile(Projectile* projectile);
        void Draw();
        void UpdateProjectiles(float dt);
    private:
        std::vector<Projectile*> m_Projectiles;
};