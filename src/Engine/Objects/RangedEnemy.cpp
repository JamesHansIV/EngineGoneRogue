#include "RangedEnemy.h"

RangedEnemy::~RangedEnemy() {
    delete m_ProjectileManager;
}

void RangedEnemy::Draw() {
    m_CurrentState->Draw();
    m_Health->Draw(GetX(), GetY(), GetWidth());
    GetProjectileManager()->Draw();
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

    GetProjectileManager()->UpdateProjectiles(dt);
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
}

void RangedEnemy::Clean() {}
