#include "Engine/State/SlimeState.h"
#include "Apps/Game.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Objects/Player.h"
#include "Engine/Objects/Projectile.h"

State* SlimeHandleCollide(Slime* enemy, Collider* collidee) {
    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:
            if (dynamic_cast<Projectile*>(collidee)->PlayerOwned()) {
                return new SlimeIsHit(enemy, 10);
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

void SlimeSplit(Slime* enemy) {
    std::string id1 = enemy->GetID() + "1";
    std::string id2 = enemy->GetID() + "2";
    Properties props1 = {enemy->GetTextureID(),
                         enemy->GetTilePos(),
                         enemy->GetDstRect(),
                         enemy->GetRotation(),
                         id1,
                         enemy->GetFlip()};
    props1.DstRect.x -= (float)enemy->GetWidth() / 2;

    Properties props2 = props1;
    props2.ObjectID = id2;
    props1.DstRect.x += (float)enemy->GetWidth() / 2;

    auto* slime1 = new Slime(props1, enemy->GetPerception().w,
                             enemy->GetPerception().h, enemy->GetRange(), true);
    auto* slime2 = new Slime(props2, enemy->GetPerception().w,
                             enemy->GetPerception().h, enemy->GetRange(), true);

    dynamic_cast<Game*>(Application::Get())->AddObject(slime1);
    dynamic_cast<Game*>(Application::Get())->AddObject(slime2);

    ColliderHandler::GetInstance()->AddCollider(slime1);
    ColliderHandler::GetInstance()->AddCollider(slime2);
}

void BigSlimeSelectAnimation(Slime* enemy, StateType type) {
    switch (type) {
        case StateType::Idle:
            enemy->GetAnimation()->SelectAnimation("SlimeIdle");
            break;
        case StateType::Dead:
            enemy->GetAnimation()->SelectAnimation("SlimeDead");
            break;
        case StateType::Attack:
            enemy->GetAnimation()->SelectAnimation("SlimeAttack");
            break;
        case StateType::IsHit:
            enemy->GetAnimation()->SelectAnimation("SlimeHit");
            break;
        default:
            break;
    }
}

void MiniSlimeSelectAnimation(Slime* enemy, StateType type) {
    switch (type) {
        case StateType::Idle:
            enemy->GetAnimation()->SelectAnimation("MiniSlimeIdle");
            break;
        case StateType::Dead:
            enemy->GetAnimation()->SelectAnimation("MiniSlimeDead");
            break;
        case StateType::Attack:
            enemy->GetAnimation()->SelectAnimation("MiniSlimeAttack");
            break;
        case StateType::IsHit:
            enemy->GetAnimation()->SelectAnimation("MiniSlimeHit");
            break;
        default:
            break;
    }
}

void SlimeSelectAnimation(Slime* enemy, StateType type) {
    if (!enemy->IsSplit()) {
        BigSlimeSelectAnimation(enemy, type);
    } else {
        MiniSlimeSelectAnimation(enemy, type);
    }
}

void SlimeIdle::Enter() {
    SlimeSelectAnimation(GetEnemy(), GetType());
}

void SlimeIdle::Exit() {}

State* SlimeIdle::Update(float dt) {
    if (GetEnemy()->TargetDetected()) {
        return new SlimeMoving(GetEnemy());
    }
    return nullptr;
}

void SlimeIdle::Draw() {
    GetEnemy()->DrawAnimation();
}

State* SlimeIdle::HandleEvent(Event* event) {
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

State* SlimeIdle::OnTargetFoundEvent(TargetFoundEvent* event) {
    return new SlimeMoving(GetEnemy());
}

State* SlimeIdle::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return SlimeHandleCollide(GetEnemy(), collidee);
}

void SlimeMoving::Enter() {}

void SlimeMoving::Exit() {}

State* SlimeMoving::Update(float dt) {
    if (!GetEnemy()->TargetDetected()) {
        return new SlimeIdle(GetEnemy());
    }
    if (GetEnemy()->MoveTowardsTarget(dt)) {
        return new SlimeAttack(GetEnemy());
    }
    return nullptr;
}

void SlimeMoving::Draw() {
    GetEnemy()->DrawAnimation();
}

State* SlimeMoving::HandleEvent(Event* event) {
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

State* SlimeMoving::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return SlimeHandleCollide(GetEnemy(), collidee);
}

void SlimeAttack::Enter() {
    SlimeSelectAnimation(GetEnemy(), GetType());
}

void SlimeAttack::Exit() {}

State* SlimeAttack::Update(float dt) {
    if (GetEnemy()->GetAnimation()->Ended()) {
        return new SlimeIdle(GetEnemy());
    }
    GetEnemy()->MoveTowardsTarget(dt);
    return nullptr;
}

void SlimeAttack::Draw() {
    GetEnemy()->DrawAnimation();
}

State* SlimeAttack::HandleEvent(Event* event) {
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

State* SlimeAttack::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    return SlimeHandleCollide(GetEnemy(), collidee);
}

void SlimeIsHit::Enter() {
    SlimeSelectAnimation(GetEnemy(), GetType());
}

void SlimeIsHit::Exit() {}

State* SlimeIsHit::Update(float dt) {
    if (GetEnemy()->GetAnimation()->Ended()) {
        return new SlimeIdle(GetEnemy());
    }
    if (GetEnemy()->GetHealth()->GetHP() <= 0) {
        return new SlimeDead(GetEnemy());
    }
    return nullptr;
}

void SlimeIsHit::Draw() {
    GetEnemy()->DrawAnimation();
}

State* SlimeIsHit::HandleEvent(Event* event) {
    EventType e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }

    return nullptr;
}

State* SlimeIsHit::OnCollideEvent(CollideEvent* event) {
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

void SlimeDead::Enter() {
    SlimeSelectAnimation(GetEnemy(), GetType());
}

void SlimeDead::Exit() {}

State* SlimeDead::Update(float dt) {
    if (GetEnemy()->GetAnimation()->Ended()) {
        if (!GetEnemy()->IsSplit()) {
            SlimeSplit(GetEnemy());
        }
        ColliderHandler::GetInstance()->RemoveCollider(GetEnemy());
        GetEnemy()->MarkForDeletion();
    }
    return nullptr;
}

void SlimeDead::Draw() {
    GetEnemy()->DrawAnimation();
}

State* SlimeDead::HandleEvent(Event* event) {
    return nullptr;
}
