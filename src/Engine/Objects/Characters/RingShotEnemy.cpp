#include "RingShotEnemy.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

RingShotEnemy::RingShotEnemy(Properties& props, RangedEnemyStats stats,
                             float outerRadius, float innerRadius,
                             int shotCount)
    : RangedEnemy(props, stats),
      m_OuterRadius(outerRadius),
      m_InnerRadius(innerRadius),
      m_ShotCount(shotCount) {
    Init();
}

RingShotEnemy::RingShotEnemy(Collider* rhs, RangedEnemyStats stats,
                             float outerRadius, float innerRadius,
                             int shotCount)
    : RangedEnemy(rhs, stats),
      m_OuterRadius(outerRadius),
      m_InnerRadius(innerRadius),
      m_ShotCount(shotCount) {
    Init();
}

void RingShotEnemy::Init() {
    ChangeState(new RangedEnemyIdle(this));
    SetHealth(new Health(100));
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

void RingShotEnemy::Shoot() {
    Properties const props = {
        "weapons", {6, 0, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot(RangedAttackInfo{
        GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
        GetTarget()->GetMidPointY(), props, 9, m_ShotCount});
}

void RingShotEnemy::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
