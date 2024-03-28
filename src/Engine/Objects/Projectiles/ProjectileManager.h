#pragma once

#include "Projectile.h"

using p_iterator = std::vector<Projectile*>::iterator;

class ProjectileManager {
   public:
    ProjectileManager() = default;
    ~ProjectileManager();
    void AddProjectile(Projectile* projectile);
    p_iterator RemoveProjectile(Projectile* projectile, p_iterator it);
    void Clean();
    void Draw();
    void UpdateProjectiles(float dt);

    inline static ProjectileManager* GetInstance() {
        return s_instance = (s_instance != nullptr) ? s_instance
                                                    : new ProjectileManager();
    }

   private:
    std::vector<Projectile*> m_projectiles;
    static ProjectileManager* s_instance;
};
