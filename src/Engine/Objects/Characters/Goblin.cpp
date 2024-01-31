#include "Goblin.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

Goblin::Goblin(Properties& props, int perceptionWidth, int perceptionHeight,
               float range, int fireInterval, float spread)
    : RangedEnemy(props, perceptionWidth, perceptionHeight, range, fireInterval,
                  new Burst(5, 70)),
      m_Spread(spread) {
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

void Goblin::Draw() {
    RangedEnemy::Draw();
}

void Goblin::Update(float dt) {
    GetBurst()->Update(dt);
    RangedEnemy::Update(dt);
}

void Goblin::Shoot() {
    float const delta_y = GetTarget()->GetMidPointY() - GetY();
    float const delta_x = GetTarget()->GetMidPointX() - GetX();

    float const center_angle = atan2(delta_y, delta_x);
    float const range_start = center_angle - m_Spread / 2;

    float const offset =
        static_cast<float>(rand() % static_cast<int>(m_Spread * 100)) / 100;

    float const angle = range_start + offset;

    Properties props = {
        "weapons", {6, 1, 16, 16}, {GetX(), GetY(), 12, 12}, angle};

    auto* bullet = new Projectile(props, 3, angle);
    GetProjectileManager()->AddProjectile(bullet);
    ColliderHandler::GetInstance()->AddCollider(bullet);
}

void Goblin::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}

void Goblin::Clean() {}
