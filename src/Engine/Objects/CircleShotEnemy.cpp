#include "CircleShotEnemy.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

CircleShotEnemy::CircleShotEnemy(Properties& props, int perceptionWidth,
                                 int perceptionHeight, float range,
                                 int fireInterval, float outerRadius,
                                 float innerRadius, int shotCount)
    : RangedEnemy(props, perceptionWidth, perceptionHeight, range, fireInterval,
                  nullptr),
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
}

void CircleShotEnemy::Draw() {
    RangedEnemy::Draw();
}

void CircleShotEnemy::Update(float dt) {
    RangedEnemy::Update(dt);
}

void CircleShotEnemy::Shoot() {
    float const target_x = GetTarget()->GetMidPointX();
    float const target_y = GetTarget()->GetMidPointY();
    float const delta_y = target_y - GetY();
    float const delta_x = target_x - GetX();

    float angle = 0;
    float bullet_separation = 2 * M_PI / m_ShotCount;
    Projectile* bullet;
    Vector2D bullet_pos;

    for (float i = 0; i < 2 * M_PI; i += bullet_separation) {
        angle = i * 180 / M_PI;

        bullet_pos.X = m_OuterRadius * cos(i) + GetX();
        bullet_pos.Y = m_OuterRadius * sin(i) + GetY();

        Properties props = {"weapons",
                            {6, 0, 16, 16},
                            {bullet_pos.X, bullet_pos.Y, 12, 12},
                            angle};

        bullet = new Projectile(props, 3, i);
        GetProjectileManager()->AddProjectile(bullet);
        ColliderHandler::GetInstance()->AddCollider(bullet);
    }

    for (float i = bullet_separation / 2; i < 2 * M_PI;
         i += bullet_separation) {
        angle = i * 180 / M_PI;

        bullet_pos.X = m_InnerRadius * cos(i) + GetX();
        bullet_pos.Y = m_InnerRadius * sin(i) + GetY();

        Properties props = {"weapons",
                            {6, 0, 16, 16},
                            {bullet_pos.X, bullet_pos.Y, 12, 12},
                            angle};

        bullet = new Projectile(props, 3, i);
        GetProjectileManager()->AddProjectile(bullet);
        ColliderHandler::GetInstance()->AddCollider(bullet);
    }
}

void CircleShotEnemy::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}

void CircleShotEnemy::Clean() {}
