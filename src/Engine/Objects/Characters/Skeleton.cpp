#include "Skeleton.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/State/RangedEnemyState.h"

Skeleton::Skeleton(Properties& props, const RangedEnemyStats& stats)
    : RangedEnemy(props, stats) {
    Init();
}

Skeleton::Skeleton(Collider* rhs, RangedEnemyStats stats)
    : RangedEnemy(rhs, stats) {
    Init();
}

void Skeleton::Init() {
    ChangeState(new RangedEnemyIdle(this));
    SetHealth(new Health(m_stats.health));
    SetAttack(new RangedAttack(CreateSpreadBullets, GetFireInterval()));
}

void Skeleton::Draw() {
    RangedEnemy::Draw();
    GetAttack()->Draw();
}

void Skeleton::Update(float dt) {
    RangedEnemy::Update(dt);
    GetAttack()->Update(dt);
}

void Skeleton::Shoot() {
    Properties const props = {
        "weapons", {5, 3, 16, 16}, {GetX(), GetY(), 16, 16}};

    GetAttack()->Shoot(RangedAttackInfo{GetMidPointX(), GetMidPointY(),
                                        GetTarget()->GetMidPointX(),
                                        GetTarget()->GetMidPointY(), props, 9,
                                        m_stats.spreadCount, m_stats.spread});
}

void Skeleton::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
