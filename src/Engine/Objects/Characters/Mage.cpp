#include "Mage.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Projectiles/RotatingBullet.h"
#include "Engine/State/RangedEnemyState.h"

Mage::Mage(Properties& props, const RangedEnemyStats& stats, int bulletCount)
    : RangedEnemy(props, stats), m_BulletCount(bulletCount) {
    Init();
}

Mage::Mage(Collider& rhs, RangedEnemyStats stats, int bulletCount)
    : RangedEnemy(rhs, stats), m_BulletCount(bulletCount) {
    Init();
}

void Mage::Init() {
    ChangeState(new RangedEnemyIdle(this));
    SetHealth(new Health(100));
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

void Mage::Shoot() {
    RotatingBullet* bullet;

    Properties const props = {
        "weapons", {6, 1, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot(RangedAttackInfo{
        GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
        GetTarget()->GetMidPointY(), props, 1, m_BulletCount});
}

void Mage::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
