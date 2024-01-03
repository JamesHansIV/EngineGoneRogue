#include "Goblin.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

void GoblinStats::Init() {
    Spread = 70;
    SetBurstDuration();
    SetBurstCD();
}

void GoblinStats::SetBurstDuration() {
    BurstDuration = 40;
}

void GoblinStats::SetBurstCD() {
    BurstCD = 70;
}

Goblin::Goblin(Properties& props, int perceptionWidth, int perceptionHeight,
               float range, int fireInterval)
    : RangedEnemy(props, perceptionWidth, perceptionHeight, range,
                  fireInterval) {
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

    m_Stats.Init();
    ChangeState(new RangedEnemyIdle(this));
    SetHealth(new Health(100));
}

void Goblin::Draw() {
    RangedEnemy::Draw();
}

void Goblin::Update(float dt) {
    RangedEnemy::Update(dt);
}

void Goblin::Shoot() {
    if (m_Stats.BurstDuration > 0) {
        m_Stats.BurstDuration--;
    } else {
        if (m_Stats.BurstCD > 0) {
            m_Stats.BurstCD--;
        } else {
            m_Stats.SetBurstDuration();
            m_Stats.SetBurstCD();
        }
        return;
    }

    if (Application::Get()->GetFrame() % GetFireInterval() != 0) {
        return;
    }
    float const delta_y = GetTarget()->GetMidPointY() - GetY();
    float const delta_x = GetTarget()->GetMidPointX() - GetX();

    float const center_angle = atan2(delta_y, delta_x) * (180.0 / M_PI);
    float const range_start = center_angle - m_Stats.Spread / 2;

    float const offset = rand() % (int)m_Stats.Spread;

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
