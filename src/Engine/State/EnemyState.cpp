#include "EnemyState.h"
#include "Apps/Game.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/Grenade.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Objects/Trap.h"
#include "Engine/Objects/Weapons/MeleeWeapon.h"

// 2 ways to have a general enemystate:
// 1. Inherit from EnemyState and override the functions, reuse methods if possible
// 2. Have transition methods in the EnemyState class, with pointers to all possible states, can reuse EnemyStates when possible,
// And can replace one with a more specific state when needed.
State* EnemyHandleCollide(Enemy& enemy, Collider* collidee) {
    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:{
            auto* p = dynamic_cast<Projectile*>(collidee);
            if (p->IsPlayerOwned()) {
                if(!p->IsInHitSet(enemy.GetID())){
                    p->CollideWithEnemy();
                    p->AddtoHitSet(enemy.GetID());
                    return enemy.GetHitState(
                    dynamic_cast<Projectile*>(collidee)->GetDamage());
                }
            }
            break;
        }
        case ObjectType::Entrance: {
            enemy.UnCollide(collidee);
            break;
        }
        case ObjectType::Player:
            if (dynamic_cast<Player*>(collidee)->GetCurrentState()->GetType() ==
                StateType::Dodge) {
                break;
            }
            enemy.UnCollide(collidee);
            break;
        case ObjectType::MeleeWeapon: {
            auto* melee_weapon = dynamic_cast<class MeleeWeapon*>(collidee);
            if (melee_weapon->IsPlayerOwned()) {
                return enemy.GetHitState(melee_weapon->GetStats()->GetDamage());
            }
            break;
        }
        case ObjectType::Grenade: {
            auto* grenade = dynamic_cast<Grenade*>(collidee);
            if (grenade->GetState() == BombState::EXPLODING_DAMAGING) {
                return enemy.GetHitState(grenade->GetDamage());
            }

            enemy.UnCollide(collidee);
            break;
        }
        case ObjectType::DestructibleItem:
        case ObjectType::Trap:
        case ObjectType::Chest:
        case ObjectType::Enemy:
        case ObjectType::Collider:
            enemy.UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void EnemyIdle::Enter() {
    m_enemy.GetAnimation()->SelectAnimation("Idle");
}

void EnemyIdle::Exit() {}

State* EnemyIdle::Update(float /*dt*/) {
    if (m_enemy.TargetDetected()) {
        return m_enemy.GetMovingState();
    }
    return nullptr;
}

void EnemyIdle::Draw() {
    m_enemy.DrawAnimation();
}

State* EnemyIdle::HandleEvent(Event* event) {
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

State* EnemyIdle::OnTargetFoundEvent(TargetFoundEvent* /*event*/) {
    return m_enemy.GetMovingState();
}

State* EnemyIdle::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return EnemyHandleCollide(m_enemy, collidee);
}

void EnemyMoving::Enter() {
    m_direction == 1 ? m_enemy.GetAnimation()->SelectAnimation("Move-down")
                     : m_enemy.GetAnimation()->SelectAnimation("Move-up");
}

void EnemyMoving::Exit() {}

void EnemyMoving::SelectDirection() {
    float const dy = m_enemy.GetY() - m_enemy.GetTarget()->GetY();
    int direction = 0;

    if (dy < 0) {
        direction = 1;
    }

    if (m_direction != direction) {
        m_direction = direction;
        m_direction == 1 ? m_enemy.GetAnimation()->SelectAnimation("Move-down")
                         : m_enemy.GetAnimation()->SelectAnimation("Move-up");
    }
}

State* EnemyMoving::Update(float dt) {
    SelectDirection();
    if (!m_enemy.TargetDetected()) {
        return m_enemy.GetIdleState();
    }
    if (m_enemy.MoveTowardsTarget(dt)) {
        return m_enemy.GetAttackState();
    }
    return nullptr;
}

void EnemyMoving::Draw() {
    m_enemy.DrawAnimation();
}

State* EnemyMoving::HandleEvent(Event* event) {
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

State* EnemyMoving::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return EnemyHandleCollide(m_enemy, collidee);
}

void EnemyAttack::Enter() {}

void EnemyAttack::Exit() {}

State* EnemyAttack::Update(float dt) {
    m_enemy.MoveTowardsTarget(dt);
    m_enemy.Attack();
    return nullptr;
}

void EnemyAttack::Draw() {
    //m_enemy.DrawAnimation();
}

State* EnemyAttack::HandleEvent(Event* event) {
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

State* EnemyAttack::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return EnemyHandleCollide(m_enemy, collidee);
}

void EnemyIsHit::Enter() {
    ApplyDamage();
    m_enemy.GetAnimation()->SelectAnimation("Hit");
}

void EnemyIsHit::Exit() {}

State* EnemyIsHit::Update(float /*dt*/) {
    // TODO: m_animation causes seg fault if enemy does not have animation,
    // fix by either init to null ptr and checking
    // or adding default animation
    if (m_enemy.GetAnimation()->Ended()) {
        return m_enemy.GetIdleState();
    }
    if (m_enemy.GetHealth()->GetHP() <= 0) {
        PushNewEvent(EventType::EnemyDeathEvent, &m_enemy);
        return m_enemy.GetDeadState();
    }
    return nullptr;
}

void EnemyIsHit::Draw() {
    m_enemy.DrawAnimation();
}

State* EnemyIsHit::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }

    return nullptr;
}

State* EnemyIsHit::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile: {
            auto* projectile = dynamic_cast<Projectile*>(collidee);
            if (projectile->IsPlayerOwned()) {
                //SetDamage(projectile->GetDamage());
                //ApplyDamage();
            }
            break;
        }
        case ObjectType::Entrance: {
            m_enemy.UnCollide(collidee);
            break;
        }
        case ObjectType::Player:
        case ObjectType::Enemy:
        case ObjectType::Collider:
            m_enemy.UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void EnemyDead::Enter() {
    if (m_enemy.GetAnimation() != nullptr) {
        m_enemy.GetAnimation()->SelectAnimation("Dead");
    }
}

void EnemyDead::Exit() {}

State* EnemyDead::Update(float /*dt*/) {
    if (m_enemy.GetAnimation()->Ended()) {
        m_enemy.MarkForDeletion();
    }
    return nullptr;
}

void EnemyDead::Draw() {
    m_enemy.DrawAnimation();
}

State* EnemyDead::HandleEvent(Event* /*event*/) {
    return nullptr;
}
