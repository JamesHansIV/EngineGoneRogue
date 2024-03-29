#include "Charger.h"
#include "Engine/State/ChargerState.h"
#include "Engine/State/EnemyState.h"

Charger::Charger(Properties& props, const EnemyStats& stats)
    : Enemy(props, stats) {
    Init();
}

Charger::Charger(Collider* rhs, EnemyStats stats) : Enemy(rhs, stats) {
    Init();
}

Charger::~Charger() {
    if (m_health != nullptr) {
        delete m_health;
        m_health = nullptr;
    }
    if (m_current_state != nullptr) {
        delete m_current_state;
        m_current_state = nullptr;
    }
}

void Charger::Init() {
    SetHealth(new Health(m_stats.health));

    m_current_state = new EnemyIdle(*this);
}

void Charger::Draw() {
    m_current_state->Draw();
    Enemy::Draw();
    //Todo(Ahmni): Add flag to only show enemy health bar in dev mode.
    m_health->Draw(GetX(), GetY(), GetWidth());
}

void Charger::Update(float dt) {
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

GameObject* Charger::Copy() {
    return new Charger(this, GetEnemyStats());
}

void Charger::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    CollideEvent event(collidee);
    State* state = m_current_state->HandleEvent(&event);
    if (state != nullptr) {
        ChangeState(state);
    }
}

void Charger::Clean() {}

EnemyState* Charger::GetAttackState() {
    return new ChargerAttack(*this);
}
