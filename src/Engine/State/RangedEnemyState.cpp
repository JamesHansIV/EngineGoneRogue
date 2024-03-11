#include "RangedEnemyState.h"
#include "Engine/Application/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/GameEventManager.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Objects/Weapons/MeleeWeapon.h"
#include "Engine/utils/utils.h"

State* RangedEnemyHandleCollide(RangedEnemy* enemy, Collider* collidee) {
    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:
            if (dynamic_cast<Projectile*>(collidee)->IsPlayerOwned()) {
                return new RangedEnemyIsHit(
                    enemy, dynamic_cast<Projectile*>(collidee)->GetDamage());
            }
            break;
        case ObjectType::MeleeWeapon: {
            auto* melee_weapon = dynamic_cast<class MeleeWeapon*>(collidee);
            return new RangedEnemyIsHit(enemy,
                                        melee_weapon->GetStats().GetDamage());
            break;
        }
        case ObjectType::Entrance: {
            auto* entrance = dynamic_cast<Entrance*>(collidee);
            if (entrance->GetCurrentState()->GetType() == StateType::Closed ||
                entrance->GetCurrentState()->GetType() == StateType::Opening) {
                enemy->UnCollide(collidee);
            }
            break;
        }
        case ObjectType::Player:
            if (dynamic_cast<Player*>(collidee)->GetCurrentState()->GetType() ==
                StateType::Dodge) {
                break;
            }
        case ObjectType::Enemy:
        case ObjectType::Collider:
            enemy->UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void RangedEnemyIdle::Enter() {
    GetEnemy()->GetAnimation()->SelectAnimation("Idle");
}

void RangedEnemyIdle::Exit() {}

State* RangedEnemyIdle::Update(float /*dt*/) {
    if (GetEnemy()->TargetDetected()) {
        return new RangedEnemyMoving(GetEnemy());
    }
    return nullptr;
}

void RangedEnemyIdle::Draw() {
    GetEnemy()->DrawAnimation();
}

State* RangedEnemyIdle::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::TargetFoundEvent:
            return OnTargetFoundEvent(dynamic_cast<TargetFoundEvent*>(event));
            break;
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }
    return nullptr;
}

State* RangedEnemyIdle::OnTargetFoundEvent(TargetFoundEvent* /*event*/) {
    return new RangedEnemyMoving(GetEnemy());
}

State* RangedEnemyIdle::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return RangedEnemyHandleCollide(GetEnemy(), collidee);
}

void RangedEnemyMoving::Enter() {
    m_direction == 1 ? GetEnemy()->GetAnimation()->SelectAnimation("Move-down")
                     : GetEnemy()->GetAnimation()->SelectAnimation("Move-up");
}

void RangedEnemyMoving::Exit() {}

State* RangedEnemyMoving::Update(float dt) {
    SelectDirection();
    if (!GetEnemy()->TargetDetected()) {
        return new RangedEnemyIdle(GetEnemy());
    }
    if (GetEnemy()->MoveTowardsTarget(dt)) {
        return new RangedEnemyAttack(GetEnemy());
    }
    return nullptr;
}

void RangedEnemyMoving::Draw() {
    GetEnemy()->DrawAnimation();
}

void RangedEnemyMoving::SelectDirection() {
    float const dy = GetEnemy()->GetY() - GetEnemy()->GetTarget()->GetY();
    int direction = 0;

    if (dy < 0) {
        direction = 1;
    }

    if (m_direction != direction) {
        m_direction = direction;
        m_direction == 1
            ? GetEnemy()->GetAnimation()->SelectAnimation("Move-down")
            : GetEnemy()->GetAnimation()->SelectAnimation("Move-up");
    }
}

State* RangedEnemyMoving::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
            break;
        default:
            break;
    }
    return nullptr;
}

State* RangedEnemyMoving::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return RangedEnemyHandleCollide(GetEnemy(), collidee);
}

void RangedEnemyAttack::Enter() {
    //TODO: We need to add an idle animation for when the enemy is facing up as well
    GetEnemy()->GetAnimation()->SelectAnimation("Idle");
}

void RangedEnemyAttack::Exit() {
    GetEnemy()->GetAttack()->Reset();
}

State* RangedEnemyAttack::Update(float /*dt*/) {
    if (!GetEnemy()->TargetInRange()) {
        return new RangedEnemyIdle(GetEnemy());
    }
    GetEnemy()->Shoot();

    return nullptr;
}

void RangedEnemyAttack::Draw() {
    GetEnemy()->DrawAnimation();
}

State* RangedEnemyAttack::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
            break;
        default:
            break;
    }
    return nullptr;
}

State* RangedEnemyAttack::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return RangedEnemyHandleCollide(GetEnemy(), collidee);
}

void RangedEnemyIsHit::Enter() {
    ApplyDamage();
    GetEnemy()->GetAnimation()->SelectAnimation("Hit");
}

void RangedEnemyIsHit::Exit() {}

State* RangedEnemyIsHit::Update(float /*dt*/) {
    if (GetEnemy()->GetAnimation()->Ended()) {
        return new RangedEnemyIdle(GetEnemy());
    }
    if (GetEnemy()->GetHealth()->GetHP() <= 0) {
        PushNewEvent(EventType::EnemyDeathEvent, GetEnemy());
        return new RangedEnemyDead(GetEnemy());
    }
    return nullptr;
}

void RangedEnemyIsHit::Draw() {
    GetEnemy()->DrawAnimation();
}

State* RangedEnemyIsHit::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }

    return nullptr;
}

State* RangedEnemyIsHit::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();
    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile: {
            auto* projectile = dynamic_cast<Projectile*>(collidee);
            if (projectile->IsPlayerOwned()) {
                //SetDamage(projectile->GetDamage());
                //SDL_Log("Enemy hit: %d", projectile->GetDamage());
                //ApplyDamage();
            }

            break;
        }
        case ObjectType::Entrance: {
            if (dynamic_cast<Entrance*>(collidee)
                    ->GetCurrentState()
                    ->GetType() != StateType::Opened) {
                GetEnemy()->UnCollide(collidee);
            }
            break;
        }
        case ObjectType::Chest:
        case ObjectType::Player:
        case ObjectType::Enemy:
        case ObjectType::Collider:
            GetEnemy()->UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void RangedEnemyDead::Enter() {
    GetEnemy()->GetAnimation()->SelectAnimation("Dead");
}

void RangedEnemyDead::Exit() {}

State* RangedEnemyDead::Update(float /*dt*/) {
    if (GetEnemy()->GetAnimation()->Ended()) {
        ColliderHandler::GetInstance()->RemoveCollider(GetEnemy());
        GetEnemy()->MarkForDeletion();
    }
    return nullptr;
}

void RangedEnemyDead::Draw() {
    GetEnemy()->DrawAnimation();
}

State* RangedEnemyDead::HandleEvent(Event* /*event*/) {
    return nullptr;
}
