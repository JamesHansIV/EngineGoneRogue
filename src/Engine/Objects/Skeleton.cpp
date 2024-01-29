#include "Skeleton.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"
#include "Entrance.h"

Skeleton::Skeleton(Properties& props, int perceptionWidth, int perceptionHeight,
                   float range, int fireInterval, float spread, int spreadCount)
    : RangedEnemy(props, perceptionWidth, perceptionHeight, range, fireInterval,
                  nullptr),
      m_Spread(spread),
      m_SpreadCount(spreadCount) {
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
}

void Skeleton::Draw() {
    RangedEnemy::Draw();
}

void Skeleton::Update(float dt) {
    RangedEnemy::Update(dt);
}

void Skeleton::Shoot() {
    float const target_x = GetTarget()->GetMidPointX();
    float const target_y = GetTarget()->GetMidPointY();
    float const delta_y = target_y - GetY();
    float const delta_x = target_x - GetX();

    float const center_angle = atan2(delta_y, delta_x);
    float const spread_start = center_angle - m_Spread / 2;
    float const interval = m_Spread / m_SpreadCount;

    Projectile* bullet;
    float angle = 0;

    for (int i = 0; i < m_SpreadCount; i++) {
        angle = spread_start + interval * i;

        Properties props = {
            "weapons", {6, 3, 16, 16}, {GetX(), GetY(), 16, 16}, angle};

        bullet = new Projectile(props, 3, angle);
        GetProjectileManager()->AddProjectile(bullet);
        ColliderHandler::GetInstance()->AddCollider(bullet);
    }
}

void Skeleton::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}

void Skeleton::Clean() {}
