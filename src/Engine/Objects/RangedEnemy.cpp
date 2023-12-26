#include "RangedEnemy.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"
#include "Entrance.h"

RangedEnemy::RangedEnemy(Properties& props, int perceptionWidth,
                         int perceptionHeight, float range, int fireRate)
    : Enemy(props, perceptionWidth, perceptionHeight, range),
      m_FireRate(fireRate) {
    m_Animation = new Animation();
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
    m_ProjectileManager = new ProjectileManager();
}

void RangedEnemy::Draw() {
    m_CurrentState->Draw();
    m_Health->Draw(GetX(), GetY(), GetWidth());
    m_ProjectileManager->Draw();
}

void RangedEnemy::Update(float dt) {
    State* state = m_CurrentState->Update(dt);

    if (state != nullptr) {
        ChangeState(state);
    }

    m_Animation->Update();
    m_RigidBody->Update(dt);

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());

    m_ProjectileManager->UpdateProjectiles(dt);
}

void RangedEnemy::Shoot() {
    float const target_x = GetTarget()->GetMidPointX();
    float const target_y = GetTarget()->GetMidPointY();
    float const delta_y = target_y - GetY();
    float const delta_x = target_x - GetX();

    float const angle = atan2(delta_y, delta_x) * (180.0 / M_PI);

    Properties props = {
        "enemies", {6, 4, 16, 16}, {GetX(), GetY(), 10, 10}, angle};

    auto* bullet = new Projectile(props, 3, angle);
    m_ProjectileManager->AddProjectile(bullet);
    ColliderHandler::GetInstance()->AddCollider(bullet);
}

void RangedEnemy::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    CollideEvent event(collidee);
    State* state = m_CurrentState->HandleEvent(&event);

    if (state != nullptr) {
        ChangeState(state);
    }

    return;
}

void RangedEnemy::Clean() {}
