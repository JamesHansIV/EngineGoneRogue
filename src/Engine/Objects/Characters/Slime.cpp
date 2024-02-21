#include "Slime.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/State/SlimeState.h"

Slime::Slime(Properties& props, const EnemyStats& stats, bool split)
    : Enemy(props, stats), m_Split(split) {
    Init();
}

Slime::Slime(Collider* rhs, EnemyStats stats, bool split)
    : Enemy(rhs, stats), m_Split(split) {
    Init();
}

void Slime::Init() {
    SetHealth(new Health(100));

    ChangeState(new SlimeIdle(this));
}

void Slime::Draw() {
    m_CurrentState->Draw();

    //Todo(Ahmni): Add flag to only show enemy health bar in dev mode.
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
    delete m_CurrentState;
    delete m_Health;
}
