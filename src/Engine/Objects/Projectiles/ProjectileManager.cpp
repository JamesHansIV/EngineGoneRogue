#include "Engine/Objects/Projectiles/ProjectileManager.h"
#include "Engine/Objects/ColliderHandler.h"

ProjectileManager* ProjectileManager::s_instance = nullptr;

ProjectileManager::~ProjectileManager() {
    for (auto it = m_projectiles.begin(); it != m_projectiles.end();) {
        it = RemoveProjectile(*it, it);
    }
}

void ProjectileManager::AddProjectile(Projectile* projectile) {
    if (projectile == nullptr) {
        SDL_LogError(0, "Projectile is null");
        return;
    }

    m_projectiles.push_back(projectile);
}

p_iterator ProjectileManager::RemoveProjectile(Projectile* projectile,
                                               p_iterator it) {
    ColliderHandler::GetInstance()->RemoveCollider(projectile);
    (projectile)->Clean();
    delete projectile;
    return m_projectiles.erase(it);
}

void ProjectileManager::Draw() {
    for (auto* projectile : m_projectiles) {
        projectile->Draw();
    }
}

void ProjectileManager::UpdateProjectiles(float dt) {
    for (auto it = m_projectiles.begin(); it != m_projectiles.end();) {
        Projectile* projectile = *it;
        (projectile)->Update(dt);
        if ((projectile)->MarkedForDeletion()) {
            it = RemoveProjectile(projectile, it);
        } else {
            it++;
        }
    }
}
