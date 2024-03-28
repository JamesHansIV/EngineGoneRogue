#include "Slime.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/State/SlimeState.h"

Slime::Slime(Properties& props, const EnemyStats& stats, bool split)
    : Enemy(props, stats), m_split(split) {
    Init();
}

Slime::Slime(Collider* rhs, EnemyStats stats, bool split)
    : Enemy(rhs, stats), m_split(split) {
    Init();
}

Slime::~Slime() {
    if (m_health != nullptr) {
        delete m_health;
        m_health = nullptr;
    }
    if (m_current_state != nullptr) {
        delete m_current_state;
        m_current_state = nullptr;
    }
}

void Slime::Init() {
    SetHealth(new Health(m_stats.health));

    m_current_state = new SlimeIdle(this);
}

void Slime::Draw() {
    m_current_state->Draw();

    //Todo(Ahmni): Add flag to only show enemy health bar in dev mode.
    m_health->Draw(GetX(), GetY(), GetWidth());
}

void Slime::Update(float dt) {
    State* state = m_current_state->Update(dt);
    if (state != nullptr) {
        ChangeState(state);
    }

    m_rigid_body->Update(dt);
    m_animation->Update();

    SetX(m_rigid_body->Position().X);
    SetY(m_rigid_body->Position().Y);
    m_collision_box.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
}

GameObject* Slime::Copy() {
    return new Slime(this, GetEnemyStats());
}

void Slime::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    CollideEvent event(collidee);
    State* state = m_current_state->HandleEvent(&event);
    if (state != nullptr) {
        ChangeState(state);
    }
}

void Slime::Clean() {}
