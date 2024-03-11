#include "Attack.h"

#include <utility>
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Timer/Timer.h"

void RangedAttack::Update(float dt) {
    if (m_attack_pattern != nullptr) {
        m_attack_pattern->Update(dt);
    }
}

void RangedAttack::Draw() {}

void RangedAttack::Reset() {
    if (m_attack_pattern != nullptr) {
        m_attack_pattern->Reset();
    }
}

void RangedAttack::Perform() {}

void RangedAttack::Shoot(RangedAttackInfo info) {
    if ((timer.GetTicks() - m_last_attack) <= m_interval) {
        return;
    }
    if (m_attack_pattern != nullptr && !m_attack_pattern->Attack()) {
        return;
    }
    std::vector<Projectile*> const bullets = m_create_bullets(std::move(info));
    m_last_attack = timer.GetTicks();
    for (auto* bullet : bullets) {
        ProjectileManager::GetInstance()->AddProjectile(bullet);
        ColliderHandler::GetInstance()->AddCollider(bullet);
    }
}

RangedAttack::~RangedAttack() {
    delete m_attack_pattern;
}
