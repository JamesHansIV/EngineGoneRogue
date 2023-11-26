#include "RangedEnemy.h"
#include "Entrance.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"

RangedEnemy::RangedEnemy(Properties& props, int perceptionWidth, int perceptionHeight, float range, int fireRate)
    : Enemy(props, perceptionWidth, perceptionHeight, range), m_FireRate(fireRate) {
    m_Animation = new Animation();
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, {6, 2, 16, 16}, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-up", {m_TextureID, {8, 4, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Move-down", {m_TextureID, {8, 0, 16, 16}, 4, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Hit", {m_TextureID, {7, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "Dead", {m_TextureID, {6, 0, 16, 16}, 2, 50, SDL_FLIP_NONE});
    m_Animation->SelectAnimation("Idle");
    m_State.AddState(CharacterState::Idle);
    SetHealth(new Health(100));
}

void RangedEnemy::Draw() {
    m_Animation->Draw(GetDstRect());
    if (!m_State.HasState(CharacterState::Dead)) {
        m_Health->Draw(GetX(), GetY(), GetWidth());
    }
    m_ProjectileManager.Draw(); 
}

void RangedEnemy::Update(float dt) {
    m_Animation->Update();

    bool cont = ManageState(dt);
    if (!cont) {
        return;
    }

    m_RigidBody->Update(dt);
    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());

    if (m_Health->GetHP() <= 0) {
        GetState().SetState(CharacterState::Dead);
        m_Animation->SelectAnimation("Dead");
        ColliderHandler::GetInstance()->RemoveCollider(this);
        m_CollisionBox.clear();
    }
    m_ProjectileManager.UpdateProjectiles(dt);
}

bool RangedEnemy::ManageState(float dt) {
    if (GetState().HasState(CharacterState::Dead)) {
        if (m_Animation->Ended()) {
            GetState().SetState(CharacterState::ToBeDestroyed);
        }
        return false;
    }

    if (m_State.HasState(CharacterState::Attack)) {
        if (Application::Get()->GetFrame() % m_FireRate == 0) {
            Shoot();
        }
        if (!TargetDetected()) {
            m_State.RemoveState(CharacterState::Attack);
        }
        return true;
    }

    if (TargetDetected()) {
        SelectMoveAnimation();
        SDL_Log("ranged enemy vel before move (%f, %f)", GetRigidBody()->Velocity().X, GetRigidBody()->Velocity().Y);
        bool inRange = MoveTowardsTarget(dt, GetRange());
        SDL_Log("ranged enemy pos after move (%f, %f)", GetRigidBody()->Velocity().X, GetRigidBody()->Velocity().Y); 

        if (inRange) {
            m_State.AddState(CharacterState::Attack);
        }
    } else {
        m_State.AddState(CharacterState::Idle);
        m_Animation->SelectAnimation("Idle");
    } 
    return true;
}

void RangedEnemy::SelectMoveAnimation() {
    if (m_State.HasState(CharacterState::Idle)) {
        float dy = GetY() - GetTarget()->GetY();
        if (dy < 0) {
            m_Animation->SelectAnimation("Move-down");
        } else {
            m_Animation->SelectAnimation("Move-up");
        }
        m_State.RemoveState(CharacterState::Idle);
    }
}

void RangedEnemy::Shoot() {
    float target_x = GetTarget()->GetMidPointX(); 
    float target_y = GetTarget()->GetMidPointY(); 
    float delta_y = target_y - GetY();
    float delta_x = target_x - GetX();

    float angle = atan2(delta_y, delta_x) * (180.0 / M_PI);

    Properties props = {
        "enemies",
        { 6, 4, 16, 16 },
        { GetX(), GetY(), 10, 10 },
        angle};

    Projectile* bullet = new Projectile(props, 3, angle);
    m_ProjectileManager.AddProjectile(bullet);
    ColliderHandler::GetInstance()->AddCollider(bullet);
}

void RangedEnemy::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            UnCollide(collidee);
            break;
        case ObjectType::Enemy:
            UnCollide(collidee);
            break;
        case ObjectType::MeleeWeapon:
            UnCollide(collidee);
            break;
        case ObjectType::Projectile: {
            auto* projectile = dynamic_cast<Projectile*>(collidee);
            if (projectile->PlayerOwned() && !GetState().HasState(CharacterState::IsHit)) {
                GetState().AddState(CharacterState::IsHit);
                m_Animation->SelectAnimation("Hit");
            }
            break;
        }
        case ObjectType::Entrance: {
            auto* entrance = dynamic_cast<Entrance*>(collidee);
            assert(entrance != nullptr);
            if (!entrance->GetState().HasState(EntranceState::Open)) {
                UnCollide(collidee);
            }
            break;
        }
        case ObjectType::Collider:
            UnCollide(collidee);
            break;
        default:
            break;
    }
}

void RangedEnemy::Clean() {

}