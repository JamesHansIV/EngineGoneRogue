#include "HelixEnemy.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/Characters/RangedEnemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Projectiles/HelixBullet.h"
#include "Engine/State/RangedEnemyState.h"

HelixEnemy::HelixEnemy(Properties& props, RangedEnemyStats const& stats)
    : RangedEnemy(props, stats) {
    Init();
}

HelixEnemy::HelixEnemy(Collider* rhs, RangedEnemyStats stats)
    : RangedEnemy(rhs, stats) {
    Init();
}

void HelixEnemy::Init() {
    m_current_state = new RangedEnemyIdle(this);
    SetHealth(new Health(m_stats.health));
    SetAttack(new RangedAttack(CreateHelixBullets, GetFireInterval(),
                               new Burst(3, 50)));
}

void HelixEnemy::Draw() {
    RangedEnemy::Draw();
    GetAttack()->Draw();
}

void HelixEnemy::Update(float dt) {
    GetAttack()->Update(dt);
    RangedEnemy::Update(dt);
}

GameObject* HelixEnemy::Copy() {
    return new HelixEnemy(this, GetRangedEnemyStats());
}

void HelixEnemy::Shoot() {
    Properties const props("weapons", {5, 1, 16, 16}, {GetX(), GetY(), 12, 12});

    GetAttack()->Shoot(RangedAttackInfo{
        GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
        GetTarget()->GetMidPointY(), props, kDefaultHitAnimationInfo, 2});
}

void HelixEnemy::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
