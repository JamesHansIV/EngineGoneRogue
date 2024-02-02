#include "Attack.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"

void RangedAttack::Update(float dt) {
    m_ProjectileManager.UpdateProjectiles(dt);
    if (m_AttackPattern != nullptr) {
        m_AttackPattern->Update(dt);
    }
}

void RangedAttack::Draw() {
    m_ProjectileManager.Draw();
}

void RangedAttack::Reset() {
    if (m_AttackPattern != nullptr) {
        m_AttackPattern->Reset();
    }
}

void RangedAttack::Perform() {}

void RangedAttack::Shoot(RangedAttackInfo info) {
    if (Application::Get()->GetFrame() % m_Interval != 0) {
        return;
    }
    if (m_AttackPattern != nullptr && !m_AttackPattern->Attack()) {
        return;
    }
    std::vector<Projectile*> bullets = m_CreateBullets(info);
    for (auto* bullet : bullets) {
        m_ProjectileManager.AddProjectile(bullet);
        ColliderHandler::GetInstance()->AddCollider(bullet);
    }
}
