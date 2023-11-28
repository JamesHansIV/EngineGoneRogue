#include "Engine/Objects/ProjectileManager.h"
#include "Engine/Objects/ColliderHandler.h"


ProjectileManager::~ProjectileManager() {
    for (auto it = m_Projectiles.begin(); it != m_Projectiles.end();) {
        it = RemoveProjectile(*it, it);
    }
}

void ProjectileManager::AddProjectile(Projectile* projectile) {
    if (projectile == nullptr) {
        SDL_LogError(0, "Projectile is null");
        return;
    }

    m_Projectiles.push_back(projectile);
}

p_iterator ProjectileManager::RemoveProjectile(Projectile* projectile, p_iterator it) {
    ColliderHandler::GetInstance()->RemoveCollider(projectile);
    (projectile)->Clean();
    delete projectile;
    return m_Projectiles.erase(it);
}

void ProjectileManager::Draw() {
    for (auto* projectile : m_Projectiles) {
        projectile->Draw();
    }
}

void ProjectileManager::UpdateProjectiles(float dt) {
    for (auto it = m_Projectiles.begin(); it != m_Projectiles.end();) {
        Projectile* projectile = *it;
        (projectile)->Update(dt);
        if ((projectile)->IsMarkedForDeletion()) {
            it = RemoveProjectile(projectile, it);
        } else {
            it++;
        }
    }
}