#include "Mage.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Projectiles/RotatingBullet.h"
#include "Engine/State/RangedEnemyState.h"

Mage::Mage(Properties& props, const RangedEnemyStats& stats, int bulletCount)
    : RangedEnemy(props, stats), m_BulletCount(bulletCount) {
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, {6, 2, 16, 16}, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-up", {m_TextureID, {8, 4, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-down", {m_TextureID, {8, 0, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Hit", {m_TextureID, {7, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "Dead", {m_TextureID, {6, 0, 16, 16}, 2, 50, SDL_FLIP_NONE});

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

    Properties const props = {"weapons", {6, 1, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot({GetMidPointX(), GetMidPointY(),
                        GetTarget()->GetMidPointX(),
                        GetTarget()->GetMidPointY(), props, 3, m_BulletCount});
}

void Mage::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}

void Mage::Clean() {}
