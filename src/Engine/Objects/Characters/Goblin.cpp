#include "Goblin.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

Goblin::Goblin(Properties& props, const RangedEnemyStats& stats)
    : RangedEnemy(props, stats) {
    Init();
}

Goblin::Goblin(Collider& rhs, RangedEnemyStats stats)
    : RangedEnemy(rhs, stats) {
    Init();
}

void Goblin::Init() {
    ChangeState(new RangedEnemyIdle(this));
    SetHealth(new Health(100));
    SetAttack(new RangedAttack(CreateSpreadBullet, GetFireInterval(),
                               new Burst(5, 70)));
}

void Goblin::Draw() {
    RangedEnemy::Draw();
    GetAttack()->Draw();
}

void Goblin::Update(float dt) {
    RangedEnemy::Update(dt);
    GetAttack()->Update(dt);
}

void Goblin::Shoot() {
    Properties const props = {
        "weapons", {6, 1, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot(RangedAttackInfo{
        GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
        GetTarget()->GetMidPointY(), props, 18, 1, m_stats.spread});
}

void Goblin::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
