#include "Kamikaze.h"
#include "Engine/State/EnemyState.h"
#include "Engine/State/KamikazeState.h"

Kamikaze::Kamikaze(Properties& props, const EnemyStats& stats)
    : Enemy(props, stats) {
    Init();
}

Kamikaze::Kamikaze(Collider* rhs, EnemyStats stats) : Enemy(rhs, stats) {
    Init();
}

Kamikaze::~Kamikaze() {
    if (m_health != nullptr) {
        delete m_health;
        m_health = nullptr;
    }
    if (m_current_state != nullptr) {
        delete m_current_state;
        m_current_state = nullptr;
    }
}

void Kamikaze::Init() {
    SetHealth(new Health(m_stats.health));

    m_current_state = new EnemyIdle(*this);
}

void Kamikaze::Draw() {
    m_current_state->Draw();
    Enemy::Draw();
    //Todo(Ahmni): Add flag to only show enemy health bar in dev mode.
    m_health->Draw(GetX(), GetY(), GetWidth());
}

void Kamikaze::Update(float dt) {
    State* state = m_current_state->Update(dt);
    if (state != nullptr) {
        ChangeState(state);
    }

    m_rigid_body->Update(dt);
    m_animation->Update();

    SDL_Log("Enemy speed: %f", m_stats.speed);

    SetX(m_rigid_body->Position().X);
    SetY(m_rigid_body->Position().Y);
    m_collision_box.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
}

GameObject* Kamikaze::Copy() {
    return new Kamikaze(this, GetEnemyStats());
}

void Kamikaze::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    CollideEvent event(collidee);
    State* state = m_current_state->HandleEvent(&event);
    if (state != nullptr) {
        ChangeState(state);
    }
}

void Kamikaze::Clean() {}

EnemyState* Kamikaze::GetAttackState() {
    return new KamikazeAttack(*this);
}
