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

Enemy::Enemy(Properties& props, int perceptionWidth, int perceptionHeight, float range)
    : Character(props),
      m_PerceptionWidth(perceptionWidth),
      m_PerceptionHeight(perceptionHeight),
      m_Range(range) {
}

bool Enemy::TargetDetected() {
    int const rect_left = GetX() - m_PerceptionWidth;
    int const rect_right = GetX() + GetWidth() + m_PerceptionWidth;
    int const rect_top = GetY() - m_PerceptionHeight;
    int const rect_bottom = GetY() + GetHeight() + m_PerceptionHeight;

    m_Perception = {rect_left, rect_top, rect_right - rect_left,
                    rect_bottom - rect_top};

    if (m_Target == nullptr) {
        SDL_Log("Target is null for object %s", GetID().c_str());
        assert(false);
    }

    SDL_Rect const target = m_Target->GetCollisionBox().GetRect();

    if (ColliderHandler::GetInstance()->CheckCollision(m_Perception, target)) {
        float const direction_x = m_Target->GetMidPointX() - GetMidPointX();
        float const direction_y = m_Target->GetMidPointY() - GetMidPointY();
        return true;
    }
    return false;
}

// Returns true if in target is in range
bool Enemy::MoveTowardsTarget(float  /*dt*/, float range) {
    if (TargetDetected()) {
        float direction_x = m_Target->GetMidPointX() - GetMidPointX();
        float direction_y = m_Target->GetMidPointY() - GetMidPointY();

        float const direction_length =
            sqrt(direction_x * direction_x + direction_y * direction_y);

        if (direction_length != 0) {
            direction_x /= direction_length;
            direction_y /= direction_length;
        }

        m_RigidBody->SetVelocity(Vector2D(direction_x, direction_y));
        if (direction_length <= range) {
            return true;
        }
    }
    return false;
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