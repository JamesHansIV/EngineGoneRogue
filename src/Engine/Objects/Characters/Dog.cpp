#include "Dog.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/State/RangedEnemyState.h"

Dog::Dog(Properties& props, const RangedEnemyStats& stats)
    : RangedEnemy(props, stats) {
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, {6, 2, 16, 16}, 2, 50, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-up", {m_TextureID, {8, 4, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-down", {m_TextureID, {8, 0, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Hit", {m_TextureID, {7, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "Dead", {m_TextureID, {6, 0, 16, 16}, 2, 75, SDL_FLIP_NONE});
    Init();
}

Dog::Dog(Collider& rhs, RangedEnemyStats stats) : RangedEnemy(rhs, stats) {
    Init();
}

void Dog::Init() {
    ChangeState(new RangedEnemyIdle(this));
    SetHealth(new Health(100));
    SetAttack(new RangedAttack(CreateBullet, GetFireInterval()));
}

void Dog::Draw() {
    RangedEnemy::Draw();
    GetAttack()->Draw();
}

void Dog::Update(float dt) {
    RangedEnemy::Update(dt);
    GetAttack()->Update(dt);
}

void Dog::Shoot() {
    Properties const props = {
        "enemies", {6, 4, 16, 16}, {GetX(), GetY(), 12, 12}};

    GetAttack()->Shoot({GetMidPointX(), GetMidPointY(),
                        GetTarget()->GetMidPointX(),
                        GetTarget()->GetMidPointY(), props});
}

void Dog::OnCollide(Collider* collidee) {
    RangedEnemy::OnCollide(collidee);
}
