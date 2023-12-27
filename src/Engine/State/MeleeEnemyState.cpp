#include "MeleeEnemyState.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Objects/Player.h"
#include "Engine/Objects/Projectile.h"

State* meleeEnemyHandleCollide(MeleeEnemy* enemy, Collider* collidee) {
    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:
            if (dynamic_cast<Projectile*>(collidee)->PlayerOwned()) {
                return new MeleeEnemyIsHit(enemy, 10);
            }
            break;
        case ObjectType::Entrance: {
            Entrance* entrance = dynamic_cast<Entrance*>(collidee);
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

void MeleeEnemyIdle::Enter() {
    GetEnemy()->GetAnimation()->SelectAnimation("Idle");
}

void MeleeEnemyIdle::Exit() {}

State* MeleeEnemyIdle::Update(float dt) {
    if (GetEnemy()->TargetDetected()) {
        return new MeleeEnemyMoving(GetEnemy());
    }
    return nullptr;
}

void MeleeEnemyIdle::Draw() {
    GetEnemy()->DrawAnimation();
}

State* MeleeEnemyIdle::HandleEvent(Event* event) {
    EventType e_type = event->GetEventType();

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

State* MeleeEnemyIdle::OnTargetFoundEvent(TargetFoundEvent* event) {
    return new MeleeEnemyMoving(GetEnemy());
}

State* MeleeEnemyIdle::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return meleeEnemyHandleCollide(GetEnemy(), collidee);
}

void MeleeEnemyMoving::Enter() {}

void MeleeEnemyMoving::Exit() {}

State* MeleeEnemyMoving::Update(float dt) {
    if (!GetEnemy()->TargetDetected()) {
        return new MeleeEnemyIdle(GetEnemy());
    }
    if (GetEnemy()->MoveTowardsTarget(dt)) {
        return new MeleeEnemyAttack(GetEnemy());
    }
    return nullptr;
}

void MeleeEnemyMoving::Draw() {
    GetEnemy()->DrawAnimation();
}

State* MeleeEnemyMoving::HandleEvent(Event* event) {
    EventType e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
            break;
        default:
            break;
    }
    return nullptr;
}

State* MeleeEnemyMoving::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return meleeEnemyHandleCollide(GetEnemy(), collidee);
}

void MeleeEnemyAttack::Enter() {
    GetEnemy()->GetAnimation()->SelectAnimation("Attack");
}

void MeleeEnemyAttack::Exit() {}

State* MeleeEnemyAttack::Update(float dt) {
    if (GetEnemy()->GetAnimation()->Ended()) {
        return new MeleeEnemyIdle(GetEnemy());
    }
    GetEnemy()->MoveTowardsTarget(dt);
    return nullptr;
}

void MeleeEnemyAttack::Draw() {
    GetEnemy()->DrawAnimation();
}

State* MeleeEnemyAttack::HandleEvent(Event* event) {
    EventType e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
            break;
        default:
            break;
    }
    return nullptr;
}

State* MeleeEnemyAttack::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return meleeEnemyHandleCollide(GetEnemy(), collidee);
}

void MeleeEnemyIsHit::Enter() {
    GetEnemy()->GetAnimation()->SelectAnimation("Hit");
}

void MeleeEnemyIsHit::Exit() {}

State* MeleeEnemyIsHit::Update(float dt) {
    if (GetEnemy()->GetAnimation()->Ended()) {
        return new MeleeEnemyIdle(GetEnemy());
    }
    if (GetEnemy()->GetHealth()->GetHP() <= 0) {
        return new MeleeEnemyDead(GetEnemy());
    }
    return nullptr;
}

void MeleeEnemyIsHit::Draw() {
    GetEnemy()->DrawAnimation();
}

State* MeleeEnemyIsHit::HandleEvent(Event* event) {
    EventType e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }

    return nullptr;
}

State* MeleeEnemyIsHit::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:
            if (dynamic_cast<Projectile*>(collidee)->PlayerOwned()) {
                SetDamage(10);
                ApplyDamage();
            }
            break;
        case ObjectType::Entrance: {
            if (dynamic_cast<Entrance*>(collidee)
                    ->GetCurrentState()
                    ->GetType() != StateType::Opened) {
                GetEnemy()->UnCollide(collidee);
            }
            break;
        }
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

void MeleeEnemyDead::Enter() {
    GetEnemy()->GetAnimation()->SelectAnimation("Dead");
}

void MeleeEnemyDead::Exit() {}

State* MeleeEnemyDead::Update(float dt) {
    if (GetEnemy()->GetAnimation()->Ended()) {
        ColliderHandler::GetInstance()->RemoveCollider(GetEnemy());
        GetEnemy()->MarkForDeletion();
    }
    return nullptr;
}

void MeleeEnemyDead::Draw() {
    GetEnemy()->DrawAnimation();
}

State* MeleeEnemyDead::HandleEvent(Event* event) {
    return nullptr;
}
