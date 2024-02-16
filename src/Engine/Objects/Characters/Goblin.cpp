#include "Goblin.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

Goblin::Goblin(Properties& props, const RangedEnemyStats& stats)
    : RangedEnemy(props, stats) {
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, {12, 2, 16, 16}, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-up", {m_TextureID, {13, 4, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-down",
        {m_TextureID, {13, 0, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Hit", {m_TextureID, {14, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "Dead", {m_TextureID, {12, 0, 16, 16}, 2, 50, SDL_FLIP_NONE});

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

    GetAttack()->Shoot(
        {GetMidPointX(), GetMidPointY(), GetTarget()->GetMidPointX(),
         GetTarget()->GetMidPointY(), props, 3, 1, m_stats.spread});
}

void Goblin::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
