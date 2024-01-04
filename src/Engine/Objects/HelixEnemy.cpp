#include "HelixEnemy.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/WaveBullet.h"
#include "Engine/State/RangedEnemyState.h"

HelixEnemy::HelixEnemy(Properties& props, int perceptionWidth,
                       int perceptionHeight, float range, int fireInterval)
    : RangedEnemy(props, perceptionWidth, perceptionHeight, range, fireInterval,
                  new Burst(3, 50)) {
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
}

void HelixEnemy::Draw() {
    RangedEnemy::Draw();
}

void HelixEnemy::Update(float dt) {
    GetBurst()->Update(dt);
    RangedEnemy::Update(dt);
}

void HelixEnemy::Shoot() {
    float const target_x = GetTarget()->GetMidPointX();
    float const target_y = GetTarget()->GetMidPointY();
    float const delta_y = target_y - GetY();
    float const delta_x = target_x - GetX();

    float const angle = atan2(delta_y, delta_x);

    Properties props = {
        "weapons", {6, 1, 16, 16}, {GetX(), GetY(), 12, 12}, angle};

    auto* bullet1 = new WaveBullet(props, 3, angle, false, false);
    auto* bullet2 = new WaveBullet(props, 3, angle, false, true);
    GetProjectileManager()->AddProjectile(bullet1);
    GetProjectileManager()->AddProjectile(bullet2);
    ColliderHandler::GetInstance()->AddCollider(bullet1);
    ColliderHandler::GetInstance()->AddCollider(bullet2);
}

void HelixEnemy::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}

void HelixEnemy::Clean() {}
