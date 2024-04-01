#include "Boss.h"
#include "Engine/Objects/Projectiles/ProjectileManager.h"
#include "Engine/State/BossState.h"
#include "Engine/State/EnemyState.h"

Boss::Boss(Properties& props, const EnemyStats& stats) : Enemy(props, stats) {
    Init();
}

Boss::Boss(Collider* rhs, EnemyStats stats) : Enemy(rhs, stats) {
    Init();
}

Boss::~Boss() {
    if (m_health != nullptr) {
        delete m_health;
        m_health = nullptr;
    }
    if (m_current_state != nullptr) {
        delete m_current_state;
        m_current_state = nullptr;
    }
}

void Boss::Init() {
    SetHealth(new Health(m_stats.health));

    m_current_state = new EnemyIdle(*this);
}

void Boss::Draw() {
    m_current_state->Draw();
    Enemy::Draw();
    //Todo(Ahmni): Add flag to only show enemy health bar in dev mode.
    m_health->Draw(GetX(), GetY(), GetWidth());
}

void Boss::Update(float dt) {
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

GameObject* Boss::Copy() {
    return new Boss(this, GetEnemyStats());
}

void Boss::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    CollideEvent event(collidee);
    State* state = m_current_state->HandleEvent(&event);
    if (state != nullptr) {
        ChangeState(state);
    }
}

void Boss::Clean() {}

EnemyState* Boss::GetAttackState() {
    return new BossAttack(*this);
}
