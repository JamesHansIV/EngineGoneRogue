#include "Engine/Objects/Projectile.h"

using p_iterator = std::vector<Projectile*>::iterator;

class ProjectileManager {
    public:
        ~ProjectileManager();
        void AddProjectile(Projectile* projectile);
        p_iterator RemoveProjectile(Projectile* projectile, p_iterator it);
        void Draw();
        void UpdateProjectiles(float dt);
    private:
        std::vector<Projectile*> m_Projectiles;
};