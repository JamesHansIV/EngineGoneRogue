#include "MeleeEnemy.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Player.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Renderer/Renderer.h"

#include <cmath>

MeleeEnemy::MeleeEnemy(Properties& props, int perceptionWidth, int perceptionHeight, float range)
    : Enemy(props, perceptionWidth, perceptionHeight, range) {
    m_Animation = new Animation();
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, {0, 2, 16, 16}, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "Hit", {m_TextureID, {1, 0, 16, 16}, 3, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "Attack", {m_TextureID, {2, 0, 16, 16}, 8, 15, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "Dead", {m_TextureID, {0, 0, 16, 16}, 2, 50, SDL_FLIP_NONE});
    m_Animation->SelectAnimation("Idle");
    SetHealth(new Health(100));
}

void MeleeEnemy::Draw() {
    m_Animation->Draw(GetDstRect());

    if (!m_State.HasState(CharacterState::Dead)) {
        m_Health->Draw(GetX(), GetY(), GetWidth());
    }
}

void MeleeEnemy::Update(float dt) {
    if (!ManageState(dt)) {
        return;
    }
    
    m_Animation->Update();

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
}

bool MeleeEnemy::ManageState(float dt) {
    if (GetState().HasState(CharacterState::Dead)) {
        if (m_Animation->Ended()) {
            GetState().SetState(CharacterState::ToBeDestroyed);
        }
        m_Animation->Update();
        return false;
    }
   
    if (GetState().HasState(CharacterState::IsHit) && m_Animation->Ended()) {
        GetState().RemoveState(CharacterState::IsHit);
        m_Animation->SelectAnimation("Idle");
        return true;
    }
    
    bool inRange = MoveTowardsTarget(dt, 45.0f);
    if (!GetState().HasState(CharacterState::Attack) && inRange) {
        SDL_Log("Adding attack state");
        GetState().AddState(CharacterState::Attack);
        m_Animation->SelectAnimation("Attack");
    } else if (m_Animation->Ended()) {
        SDL_Log("removing attack state");
        GetState().RemoveState(CharacterState::Attack);
        m_Animation->SelectAnimation("Idle");
    }

    return true;
}

void MeleeEnemy::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
}

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            UnCollide(collidee);
            SDL_Log("enemy collide with player");
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

void MeleeEnemy::Clean() {
    delete m_Animation;
}