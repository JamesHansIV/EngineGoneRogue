#include "Slime.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/State/SlimeState.h"

Slime::Slime(Properties& props, int perceptionWidth, int perceptionHeight,
             float range, bool split)
    : Enemy(props, perceptionWidth, perceptionHeight, range), m_Split(split) {
    m_Animation = new Animation();
    m_Animation->AddAnimation(
        "MiniSlimeIdle",
        {m_TextureID, {0, 2, 16, 16}, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "MiniSlimeHit", {m_TextureID, {1, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "MiniSlimeAttack",
        {m_TextureID, {2, 0, 16, 16}, 8, 15, SDL_FLIP_NONE, false, 2, 5});
    m_Animation->AddAnimation(
        "MiniSlimeDead", {m_TextureID, {0, 0, 16, 16}, 2, 50, SDL_FLIP_NONE});

    m_Animation->AddAnimation(
        "SlimeIdle", {m_TextureID, {3, 2, 16, 16}, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "SlimeHit", {m_TextureID, {4, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "SlimeAttack",
        {m_TextureID, {5, 0, 16, 16}, 8, 15, SDL_FLIP_NONE, false, 2, 5});
    m_Animation->AddAnimation(
        "SlimeDead", {m_TextureID, {3, 0, 16, 16}, 2, 50, SDL_FLIP_NONE});
    SetHealth(new Health(100));

    ChangeState(new SlimeIdle(this));
}

void Slime::Draw() {
    m_CurrentState->Draw();

    m_Health->Draw(GetX(), GetY(), GetWidth());
}

void Slime::Update(float dt) {
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

void Slime::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    CollideEvent event(collidee);
    State* state = m_CurrentState->HandleEvent(&event);
    if (state != nullptr) {
        ChangeState(state);
    }
}

void Slime::Clean() {
    delete m_Animation;
}
