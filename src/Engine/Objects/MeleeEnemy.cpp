#include "MeleeEnemy.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/State/MeleeEnemyState.h"

MeleeEnemy::MeleeEnemy(Properties& props, int perceptionWidth,
                       int perceptionHeight, float range)
    : Enemy(props, perceptionWidth, perceptionHeight, range) {
    m_Animation = new Animation();
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, {0, 2, 16, 16}, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Hit", {m_TextureID, {1, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "Attack",
        {m_TextureID, {2, 0, 16, 16}, 8, 15, SDL_FLIP_NONE, false, 2, 5});
    m_Animation->AddAnimation(
        "Dead", {m_TextureID, {0, 0, 16, 16}, 2, 50, SDL_FLIP_NONE});
    SetHealth(new Health(100));

    ChangeState(new MeleeEnemyIdle(this));
}

void MeleeEnemy::Draw() {
    m_CurrentState->Draw();

    m_Health->Draw(GetX(), GetY(), GetWidth());
}

void MeleeEnemy::Update(float dt) {
    State* state = m_CurrentState->Update(dt);
    if (state != nullptr) {
        ChangeState(state);
    }

    m_RigidBody->Update(dt);
    m_Animation->Update();

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
}

void MeleeEnemy::OnCollide(Collider* collidee) {
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

void MeleeEnemy::Clean() {
    delete m_Animation;
}
