#include "Skeleton.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/State/RangedEnemyState.h"

Skeleton::Skeleton(Properties& props, const RangedEnemyStats& stats)
    : RangedEnemy(props, stats) {
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
        "weapons", {6, 3, 16, 16}, {GetX(), GetY(), 16, 16}};

    GetAttack()->Shoot({GetMidPointX(), GetMidPointY(),
                        GetTarget()->GetMidPointX(),
                        GetTarget()->GetMidPointY(), props, 3,
                        m_stats.spreadCount, m_stats.spread});
}

void Skeleton::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
