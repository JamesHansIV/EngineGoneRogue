#include "Dog.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/Characters/RangedEnemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

Dog::Dog(Properties& props, const RangedEnemyStats& stats)
    : RangedEnemy(props, stats) {
    Init();
}

Dog::Dog(Collider* rhs, RangedEnemyStats stats) : RangedEnemy(rhs, stats) {
    Init();
}

void Dog::Init() {
    m_current_state = new RangedEnemyIdle(this);
    SetHealth(new Health(m_stats.health));
    SetAttack(new RangedAttack(CreateBullet, GetFireInterval()));
}

void Dog::Draw() {
    RangedEnemy::Draw();
    GetAttack()->Draw();
}

void Dog::Update(float dt) {
    RangedEnemy::Update(dt);
    GetAttack()->Update(dt);
}

GameObject* Dog::Copy() {
    return new Dog(this, GetRangedEnemyStats());
}

void Dog::Shoot() {
    Properties const props = {
        "enemies", {6, 4, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot(RangedAttackInfo{
        GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
        GetTarget()->GetMidPointY(), props, kDefaultHitAnimationInfo, 18});
}

void Dog::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
