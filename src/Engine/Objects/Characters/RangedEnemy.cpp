#include "RangedEnemy.h"

RangedEnemy::RangedEnemy(Collider* rhs, RangedEnemyStats stats)
    : Enemy(rhs, stats), m_stats(stats) {}

RangedEnemy::~RangedEnemy() = default;

void RangedEnemy::Draw() {
    m_current_state->Draw();
    m_health->Draw(GetX(), GetY(), GetWidth());
}

void RangedEnemy::Update(float dt) {
    State* state = m_current_state->Update(dt);

    if (state != nullptr) {
        ChangeState(state);
    }

    m_animation->Update();
    m_rigid_body->Update(dt);

    SetX(m_rigid_body->Position().X);
    SetY(m_rigid_body->Position().Y);
    m_collision_box.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
}

void RangedEnemy::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    CollideEvent event(collidee);
    State* state = m_current_state->HandleEvent(&event);

    if (state != nullptr) {
        ChangeState(state);
    }
}

void RangedEnemy::Clean() {
    delete m_attack;
    delete m_health;
}
