#include "RingShotEnemy.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/Characters/RangedEnemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

RingShotEnemy::RingShotEnemy(Properties& props, RangedEnemyStats stats,
                             float outerRadius, float innerRadius,
                             int shotCount)
    : RangedEnemy(props, stats),
      m_outer_radius(outerRadius),
      m_inner_radius(innerRadius),
      m_shot_count(shotCount) {
    Init();
}

RingShotEnemy::RingShotEnemy(Collider* rhs, RangedEnemyStats stats,
                             float outerRadius, float innerRadius,
                             int shotCount)
    : RangedEnemy(rhs, stats),
      m_outer_radius(outerRadius),
      m_inner_radius(innerRadius),
      m_shot_count(shotCount) {
    Init();
}

void RingShotEnemy::Init() {
    m_current_state = new RangedEnemyIdle(this);
    SetHealth(new Health(m_stats.health));
    SetAttack(new RangedAttack(CreateRingBullets, GetFireInterval()));
}

void RingShotEnemy::Draw() {
    RangedEnemy::Draw();
    GetAttack()->Draw();
}

void RingShotEnemy::Update(float dt) {
    RangedEnemy::Update(dt);
    GetAttack()->Update(dt);
}

GameObject* RingShotEnemy::Copy() {
    return new RingShotEnemy(this, GetRangedEnemyStats());
}

void RingShotEnemy::Shoot() {
    Properties const props = {
        "weapons", {5, 0, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot(RangedAttackInfo{
        GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
        GetTarget()->GetMidPointY(), props, kDefaultHitAnimationInfo, 9,
        m_shot_count});
}

void RingShotEnemy::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
