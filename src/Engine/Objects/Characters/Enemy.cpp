#include "Enemy.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Environment/Entrance.h"

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

Enemy::Enemy(Properties& props, EnemyStats const& stats)
    : Character(props), m_stats(stats) {}

Enemy::Enemy(Collider* rhs, EnemyStats stats) : Character(rhs), m_stats(stats) {
    SDL_Log("enemy copy constructor: %s", rhs->GetID().c_str());
}

bool Enemy::TargetInRange() {
    if (GetTarget() == nullptr) {
        return false;
    }
    const Rect range = {GetMidPointX() - m_stats.range,
                        GetMidPointY() - m_stats.range,
                        static_cast<int>(m_stats.range) * 2,
                        static_cast<int>(m_stats.range) * 2};

    const Rect rect1 = GetTarget()->GetDstRect();

    return ColliderHandler::CheckCollision(rect1, range);
}

bool Enemy::TargetDetected() {
    float const rect_x = GetX() - m_stats.perceptionWidth;
    int const rect_w = GetWidth() + 2 * m_stats.perceptionWidth;
    float const rect_y = GetY() - m_stats.perceptionHeight;
    int const rect_h = GetHeight() + 2 * m_stats.perceptionHeight;

    m_Perception = {rect_x, rect_y, rect_w, rect_h};

    if (m_Target == nullptr) {
        SDL_Log("Target is null for object %s", GetID().c_str());
        return false;
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
bool Enemy::MoveTowardsTarget(float dt) {
    if (TargetDetected()) {
        float direction_x = m_Target->GetMidPointX() - GetMidPointX();
        float direction_y = m_Target->GetMidPointY() - GetMidPointY();

        float const direction_length =
            sqrt(direction_x * direction_x + direction_y * direction_y);

        Vector2D direction;
        if (direction_length != 0) {

            direction.X = direction_x /= direction_length;
            direction.Y = direction_y /= direction_length;
        }

        m_RigidBody->ApplyVelocity(direction * dt * m_stats.speed);
        if (direction_length <= m_stats.range) {
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
        case ObjectType::Enemy:
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
    delete m_Health;
}
