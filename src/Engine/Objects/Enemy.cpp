#include "Enemy.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Objects/Player.h"
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

Enemy::Enemy(Properties& props, int perceptionWidth, int perceptionHeight,
             float range)
    : Character(props),
      m_PerceptionWidth(perceptionWidth),
      m_PerceptionHeight(perceptionHeight),
      m_Range(range) {}

bool Enemy::TargetInRange() {
    Rect range = {GetMidPointX() - m_Range, GetMidPointY() - m_Range,
                  (int)m_Range * 2, (int)m_Range * 2};
    if (ColliderHandler::CheckCollision(GetTarget()->GetDstRect(), range)) {
        return true;
    }
}

bool Enemy::TargetDetected() {
    float const rect_x = GetX() - m_PerceptionWidth;
    int const rect_w = GetWidth() + 2 * m_PerceptionWidth;
    float const rect_y = GetY() - m_PerceptionHeight;
    int const rect_h = GetHeight() + 2 * m_PerceptionHeight;

    m_Perception = {rect_x, rect_y, rect_w, rect_h};

    if (m_Target == nullptr) {
        SDL_Log("Target is null for object %s", GetID().c_str());
        assert(false);
    }

    Rect const target = m_Target->GetDstRect();

    if (ColliderHandler::GetInstance()->CheckCollision(m_Perception, target)) {
        float const direction_x = m_Target->GetMidPointX() - GetMidPointX();
        float const direction_y = m_Target->GetMidPointY() - GetMidPointY();
        return true;
    }
    return false;
}

// Returns true if in target is in range
bool Enemy::MoveTowardsTarget(float /*dt*/) {
    if (TargetDetected()) {
        float direction_x = m_Target->GetMidPointX() - GetMidPointX();
        float direction_y = m_Target->GetMidPointY() - GetMidPointY();

        float const direction_length =
            sqrt(direction_x * direction_x + direction_y * direction_y);

        if (direction_length != 0) {
            direction_x /= direction_length;
            direction_y /= direction_length;
        }

        m_RigidBody->ApplyVelocity(
            Vector2D(direction_x * 0.5, direction_y * 0.5));
        if (direction_length <= m_Range) {
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
            if (entrance->GetCurrentState()->GetType() != StateType::Opened) {
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
