#include "CircleShotEnemy.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

CircleShotEnemy::CircleShotEnemy(Properties& props, RangedEnemyStats stats,
                                 float outerRadius, float innerRadius,
                                 int shotCount)
    : RangedEnemy(props, stats),
      m_OuterRadius(outerRadius),
      m_InnerRadius(innerRadius),
      m_ShotCount(shotCount) {
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, {9, 2, 16, 16}, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-up", {m_TextureID, {11, 4, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-down",
        {m_TextureID, {11, 0, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Hit", {m_TextureID, {10, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "Dead", {m_TextureID, {9, 0, 16, 16}, 2, 50, SDL_FLIP_NONE});

    ChangeState(new RangedEnemyIdle(this));
    SetHealth(new Health(100));
    SetAttack(new RangedAttack(CreateRingBullets, GetFireInterval()));
}

void CircleShotEnemy::Draw() {
    RangedEnemy::Draw();
    GetAttack()->Draw();
}

void CircleShotEnemy::Update(float dt) {
    RangedEnemy::Update(dt);
    GetAttack()->Update(dt);
}

void CircleShotEnemy::Shoot() {
    Properties const props = {
        "weapons", {6, 0, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot(RangedAttackInfo{
        GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
        GetTarget()->GetMidPointY(), props, 9, m_ShotCount});
}

void CircleShotEnemy::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}

void CircleShotEnemy::Clean() {
    delete m_Attack;
}
