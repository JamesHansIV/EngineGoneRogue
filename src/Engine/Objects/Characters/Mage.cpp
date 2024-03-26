#include "Mage.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/Characters/RangedEnemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Projectiles/RotatingBullet.h"
#include "Engine/State/RangedEnemyState.h"

Mage::Mage(Properties& props, const RangedEnemyStats& stats, int bulletCount)
    : RangedEnemy(props, stats), m_bullet_count(bulletCount) {
    Init();
}

Mage::Mage(Collider* rhs, RangedEnemyStats stats, int bulletCount)
    : RangedEnemy(rhs, stats), m_bullet_count(bulletCount) {
    Init();
}

void Mage::Init() {
    m_current_state = new RangedEnemyIdle(this);
    SetHealth(new Health(m_stats.health));
    SetAttack(new RangedAttack(CreateRotatingBullets, GetFireInterval()));
}

void Mage::Draw() {
    RangedEnemy::Draw();
    GetAttack()->Draw();
}

void Mage::Update(float dt) {
    RangedEnemy::Update(dt);
    GetAttack()->Update(dt);
}

GameObject* Mage::Copy() {
    return new Mage(this, GetRangedEnemyStats());
}

void Mage::Shoot() {
    Properties const props = {
        "weapons", {5, 1, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot(RangedAttackInfo{
        GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
        GetTarget()->GetMidPointY(), props, kDefaultHitAnimationInfo, 2,
        m_bullet_count});
}

void Mage::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
