#include "Mage.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Projectiles/RotatingBullet.h"
#include "Engine/State/RangedEnemyState.h"

Mage::Mage(Properties& props, int perceptionWidth, int perceptionHeight,
           float range, int fireInterval, int bulletCount)
    : RangedEnemy(props, perceptionWidth, perceptionHeight, range, fireInterval,
                  nullptr),
      m_BulletCount(bulletCount) {
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
}

void Mage::Draw() {
    RangedEnemy::Draw();
}

void Mage::Update(float dt) {
    RangedEnemy::Update(dt);
}

void Mage::Shoot() {
    float const target_x = GetTarget()->GetMidPointX();
    float const target_y = GetTarget()->GetMidPointY();
    float const delta_y = target_y - GetY();
    float const delta_x = target_x - GetX();

    float const angle = atan2(delta_y, delta_x);

    float const bullet_separation = 2 * M_PI / m_BulletCount;
    RotatingBullet* bullet;

    for (float i = 0; i < 2 * M_PI; i += bullet_separation) {
        Properties props = {
            "weapons", {6, 1, 16, 16}, {GetX(), GetY(), 12, 12}, angle};

        bullet = new RotatingBullet(props, 3, angle, false, i);
        GetProjectileManager()->AddProjectile(bullet);
        ColliderHandler::GetInstance()->AddCollider(bullet);
    }
}

void Mage::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}

void Mage::Clean() {}
