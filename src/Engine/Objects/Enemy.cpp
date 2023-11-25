#include "Enemy.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Player.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Renderer/Renderer.h"

#include <cmath>

/*
DEVELOPING BASIC ENEMY BEHAVIOUR:

- define the enemy perception range
- check for player detection in the enemy perception range
- calculate the direction vector from enemy's position to the player's position
- apply behaviour like move towards the player, attack in perception,etc

- collision detection between the perception and player
- collision detection between the enemy and the player
- collision detection between the enemies (might need to add offset - but this might cause problems)
*/

Enemy::Enemy(Properties& props, int perceptionX, int perceptionY)
    : Character(props),
      m_PerceptionWidth(perceptionX),
      m_PerceptionHeight(perceptionY) {
    m_Animation = new Animation();
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, m_CurrentTilePos, 2, 15, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation("Dead", {"player", {0, 0, 18, 18}, 6, 8});
    m_Animation->SelectAnimation("Idle");
    SetHealth(new Health(100));
    m_MarkedForDeletion = false;
}

void Enemy::Draw() {
    if (m_TextureID == "boss") {
        m_Animation->Draw(
            {m_DstRect.x, m_DstRect.y, m_DstRect.w + 15, m_DstRect.h});
    } else {
        m_Animation->Draw({m_DstRect.x, m_DstRect.y, m_DstRect.w, m_DstRect.h});
    }

    if (!m_State.HasState(CharacterState::Dead)) {
        m_Health->Draw(GetX(), GetY(), GetWidth());
    }
}

void Enemy::Update(float dt) {
    if (GetState().HasState(CharacterState::Dead)) {
        m_Animation->Update();
        SDL_Log("enemy deadd");
        if (m_Animation->Stopped()) {
            GetState().SetState(CharacterState::ToBeDestroyed);

        }
        return;
    }
    m_Animation->Update();
    MoveTowardsTarget(dt, 0);
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

bool Enemy::TargetDetected() {
    int const rect_left = GetX() - m_PerceptionWidth;
    int const rect_right = GetX() + GetWidth() + m_PerceptionWidth;
    int const rect_top = GetY() - m_PerceptionHeight;
    int const rect_bottom = GetY() + GetHeight() + m_PerceptionHeight;

    m_Perception = {rect_left, rect_top, rect_right - rect_left,
                    rect_bottom - rect_top};

    SDL_Rect const target = m_Target->GetCollisionBox().GetRect();

    if (ColliderHandler::GetInstance()->CheckCollision(m_Perception, target)) {
        float direction_x = m_Target->GetMidPointX() - GetMidPointX();
        float direction_y = m_Target->GetMidPointY() - GetMidPointY();
        return true;
    }
    return false;
}

void Enemy::MoveTowardsTarget(float  /*dt*/, float minDistance) {
    if (TargetDetected()) {
        float direction_x = m_Target->GetMidPointX() - GetMidPointX();
        float direction_y = m_Target->GetMidPointY() - GetMidPointY();

        float const direction_length =
            sqrt(direction_x * direction_x + direction_y * direction_y);
        
        if (direction_length <= minDistance) {
            return;
        }

        if (direction_length != 0) {
            direction_x /= direction_length;
            direction_y /= direction_length;
        }

        m_RigidBody->SetVelocity(Vector2D(direction_x, direction_y));
    }
}

void Enemy::OnCollide(Collider* collidee) {
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
        case ObjectType::Projectile:
            break;
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

void Enemy::Clean() {
    delete m_Animation;
}