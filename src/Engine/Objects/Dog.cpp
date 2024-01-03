#include "Dog.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

Dog::Dog(Properties& props, int perceptionWidth, int perceptionHeight,
         float range, int fireInterval)
    : RangedEnemy(props, perceptionWidth, perceptionHeight, range,
                  fireInterval) {
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

void Dog::Draw() {
    RangedEnemy::Draw();
}

void Dog::Update(float dt) {
    RangedEnemy::Update(dt);
}

void Dog::Shoot() {
    if (Application::Get()->GetFrame() % GetFireInterval() != 0) {
        return;
    }
    float const target_x = GetTarget()->GetMidPointX();
    float const target_y = GetTarget()->GetMidPointY();
    float const delta_y = target_y - GetY();
    float const delta_x = target_x - GetX();

    float const angle = atan2(delta_y, delta_x) * (180.0 / M_PI);

    Properties props = {
        "enemies", {6, 4, 16, 16}, {GetX(), GetY(), 12, 12}, angle};

    auto* bullet = new Projectile(props, 3, angle);
    GetProjectileManager()->AddProjectile(bullet);
    ColliderHandler::GetInstance()->AddCollider(bullet);
}

void Dog::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}

void Dog::Clean() {}
