#include "Attack.h"

#include <utility>
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Timer/Timer.h"

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
    if ((timer.GetTicks() - m_LastAttack) <= m_Interval) {
        return;
    }
    if (m_AttackPattern != nullptr && !m_AttackPattern->Attack()) {
        return;
    }
    std::vector<Projectile*> const bullets = m_CreateBullets(std::move(info));
    m_LastAttack = timer.GetTicks();
    for (auto* bullet : bullets) {
        m_ProjectileManager.AddProjectile(bullet);
        ColliderHandler::GetInstance()->AddCollider(bullet);
    }
}

RangedAttack::~RangedAttack() {}
