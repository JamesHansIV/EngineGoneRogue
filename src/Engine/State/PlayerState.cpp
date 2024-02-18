#include "PlayerState.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/IObject.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"

bool IsPlayerDead(Player* player) {
    return (player->GetHealth()->GetHP() < 0);
}

void MovePlayer(Player* player, float dt) {
    float speed = 0;
    if (InputChecker::IsKeyPressed(SDLK_w)) {
        speed = player->GetStats().GetSpeed();
        player->GetRigidBody()->ApplyVelocity(Vector2D(0, -speed * dt));
    }
    if (InputChecker::IsKeyPressed(SDLK_s)) {
        speed = player->GetStats().GetSpeed();
        player->GetRigidBody()->ApplyVelocity(Vector2D(0, speed * dt));
    }
    if (InputChecker::IsKeyPressed(SDLK_a)) {
        speed = player->GetStats().GetSpeed();
        player->GetRigidBody()->ApplyVelocity(Vector2D(-speed * dt, 0));
    }
    if (InputChecker::IsKeyPressed(SDLK_d)) {
        speed = player->GetStats().GetSpeed();
        player->GetRigidBody()->ApplyVelocity(Vector2D(speed * dt, 0));
    }
}

State* UpdateAnimationDirection(Player* player,
                                const AnimationIDs& animationIDs) {
    bool const key_up = InputChecker::IsKeyPressed(SDLK_w);
    bool const key_down = InputChecker::IsKeyPressed(SDLK_s);
    bool const key_left = InputChecker::IsKeyPressed(SDLK_a);
    bool const key_right = InputChecker::IsKeyPressed(SDLK_d);

    bool const idle_y = key_up && key_down;
    bool const idle_x = key_left && key_right;
    bool const moving_up = !idle_y && key_up;
    bool const moving_down = !idle_y && key_down;
    bool const moving_left = !idle_x && key_left;
    bool const moving_right = !idle_x && key_right;

    // SDL_Log("up: %d, down: %d, left: %d, right: %d", movingUp, movingDown, movingLeft, movingRight);
    if (!(moving_up || moving_down || moving_left || moving_right)) {
        return new PlayerIdle(player);
    }

    if (moving_up && moving_right) {
        player->GetAnimation()->SelectAnimation(animationIDs.RightUp);
        player->SelectStillFrame("face-right-up");
        player->SetFlip(SDL_FLIP_NONE);
    } else if (moving_up && moving_left) {
        player->GetAnimation()->SelectAnimation(animationIDs.LeftUp);
        player->SelectStillFrame("face-right-up");
        player->SetFlip(SDL_FLIP_HORIZONTAL);
    } else if (moving_up) {
        player->GetAnimation()->SelectAnimation(animationIDs.Up);
        player->SelectStillFrame("face-up");
        player->SetFlip(SDL_FLIP_NONE);
    } else if (moving_right) {
        player->GetAnimation()->SelectAnimation(animationIDs.Right);
        player->SelectStillFrame("face-right");
        player->SetFlip(SDL_FLIP_NONE);
    } else if (moving_left) {
        player->GetAnimation()->SelectAnimation(animationIDs.Left);
        player->SelectStillFrame("face-right");
        player->SetFlip(SDL_FLIP_HORIZONTAL);
    } else if (moving_down) {
        player->GetAnimation()->SelectAnimation(animationIDs.Down);
        player->SelectStillFrame("face-down");
        player->SetFlip(SDL_FLIP_NONE);
    } else {
        player->GetAnimation()->StopAnimation();
    }
    return nullptr;
}

State* HandleEnemyCollide(Player* player, Enemy* enemy) {
    player->UnCollide(enemy);
    if (enemy->GetCurrentState()->GetType() == StateType::Attack &&
        enemy->GetAnimation()->OnKeyFrame()) {
        return new PlayerIsHit(player, 1);
    }
    return nullptr;
}

State* HandleProjectileCollide(Player* player, Projectile* projectile) {
    if (projectile->IsPlayerOwned()) {
        return nullptr;
    }
    return new PlayerIsHit(player, 10);
}

State* HandleEntranceCollide(Player* player, Entrance* entrance) {
    if (entrance->GetCurrentState()->GetType() == StateType::Closed ||
        entrance->GetCurrentState()->GetType() == StateType::Opening) {
        player->UnCollide(entrance);
    }
    return nullptr;
}

State* HandleCollide(Player* player, CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Enemy: {
            return HandleEnemyCollide(player, dynamic_cast<Enemy*>(collidee));
            break;
        }
        case ObjectType::Projectile:
            return HandleProjectileCollide(player,
                                           dynamic_cast<Projectile*>(collidee));
            break;
        case ObjectType::Entrance: {
            return HandleEntranceCollide(player,
                                         dynamic_cast<Entrance*>(collidee));
            break;
        }
        case ObjectType::Chest:
        case ObjectType::Collider:
            player->UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void PlayerIdle::Enter() {
    //GetPlayer()->GetAnimation()->SelectAnimation("PlayerIdle");
}

void PlayerIdle::Exit() {}

State* PlayerIdle::Update(float dt) {
    int const dodge_cd = GetPlayer()->GetStats().GetDodgeCd();
    if (dodge_cd > 0) {
        GetPlayer()->GetMutableStats().SetDodgeCd(dodge_cd - 1);
    }
    return PollInput(dt);
}

void PlayerIdle::Draw() {
    GetPlayer()->DrawAnimation();
}

State* PlayerIdle::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::UserEvent:
            return OnUserEvent(dynamic_cast<UserEvent*>(event));
            break;
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }
    return nullptr;
}

State* PlayerIdle::OnUserEvent(UserEvent* event) {
    if (event == nullptr) {
        return nullptr;
    }
    SDL_Event* e = event->GetSDLEvent();
    if (e == nullptr || (e->type != SDL_KEYDOWN && e->type != SDL_KEYUP)) {
        return nullptr;
    }

    switch (e->key.keysym.sym) {
        case SDLK_w:
        case SDLK_s:
        case SDLK_a:
        case SDLK_d:
            return new PlayerMoving(GetPlayer());
            break;
        default:
            break;
    }
    return nullptr;
}

State* PlayerIdle::OnCollideEvent(CollideEvent* event) {
    return HandleCollide(GetPlayer(), event);
}

State* PlayerIdle::PollInput(float /*dt*/) {
    if (InputChecker::IsKeyPressed(SDLK_w) ||
        InputChecker::IsKeyPressed(SDLK_s) ||
        InputChecker::IsKeyPressed(SDLK_a) ||
        InputChecker::IsKeyPressed(SDLK_d)) {
        return new PlayerMoving(GetPlayer());
    }
    return nullptr;
}

void PlayerMoving::Enter() {
    UpdateAnimationDirection(GetPlayer(), GetMoveAnimationIDs());
}

void PlayerMoving::Exit() {}

State* PlayerMoving::Update(float dt) {
    int const dodge_cd = GetPlayer()->GetStats().GetDodgeCd();
    if (dodge_cd > 0) {
        GetPlayer()->GetMutableStats().SetDodgeCd(dodge_cd - 1);
    }
    PollInput(dt);
    return nullptr;
}

void PlayerMoving::Draw() {
    GetPlayer()->DrawAnimation();
}

State* PlayerMoving::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::UserEvent:
            return OnUserEvent(dynamic_cast<UserEvent*>(event));
            break;
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }
    return nullptr;
}

State* PlayerMoving::OnUserEvent(UserEvent* event) {
    if (event == nullptr) {
        return nullptr;
    }
    SDL_Event* e = event->GetSDLEvent();
    if (e == nullptr || (e->type != SDL_KEYDOWN && e->type != SDL_KEYUP) ||
        e->key.repeat != 0) {
        return nullptr;
    }

    switch (e->key.keysym.sym) {
        case SDLK_w:
        case SDLK_s:
        case SDLK_a:
        case SDLK_d:
            return UpdateAnimationDirection(GetPlayer(), GetMoveAnimationIDs());
            break;
        case SDLK_LSHIFT:
            if (e->type == SDL_KEYDOWN &&
                GetPlayer()->GetStats().GetDodgeCd() <= 0) {
                return new PlayerDodge(GetPlayer());
            }
            break;
        default:
            break;
    }
    return nullptr;
}

State* PlayerMoving::OnCollideEvent(CollideEvent* event) {
    return HandleCollide(GetPlayer(), event);
}

void PlayerMoving::PollInput(float dt) {
    MovePlayer(GetPlayer(), dt);
}

void PlayerDodge::Enter() {
    SDL_Log("enter dodge state");
    GetPlayer()->GetMutableStats().SetDodgeCd(m_DodgeCD);
    UpdateAnimationDirection(GetPlayer(), GetDodgeAnimationIDs());
    Vector2D const velocity = GetPlayer()->GetRigidBody()->Velocity();
    float const dodge_speed = GetPlayer()->GetStats().GetDodgeSpeed();

    m_Velocity = velocity * dodge_speed;
}

void PlayerDodge::Exit() {}

State* PlayerDodge::Update(float /*dt*/) {
    if (GetPlayer()->GetAnimation()->Ended()) {
        return new PlayerIdle(GetPlayer());
    }
    GetPlayer()->GetRigidBody()->ApplyVelocity(m_Velocity);
    return nullptr;
}

void PlayerDodge::Draw() {
    GetPlayer()->DrawAnimation();
}

State* PlayerDodge::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }
    return nullptr;
}

State* PlayerDodge::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Entrance: {
            return HandleEntranceCollide(GetPlayer(),
                                         dynamic_cast<Entrance*>(collidee));
            break;
        }
        case ObjectType::Collider:
            GetPlayer()->UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void PlayerIsHit::Enter() {
    GetPlayer()->GetAnimation()->SelectAnimation("front-hit");
    ApplyDamage();
}

void PlayerIsHit::Exit() {}

State* PlayerIsHit::Update(float dt) {
    if (GetPlayer()->GetHealth()->GetHP() <= 0) {
        return new PlayerDead(GetPlayer());
    }
    if (GetPlayer()->GetAnimation()->Ended()) {
        return new PlayerIdle(GetPlayer());
    }
    PollInput(dt);
    return nullptr;
}

void PlayerIsHit::Draw() {
    GetPlayer()->DrawAnimation();
}

State* PlayerIsHit::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::UserEvent:
            return OnUserEvent(dynamic_cast<UserEvent*>(event));
            break;
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }
    return nullptr;
}

State* PlayerIsHit::OnUserEvent(UserEvent* /*event*/) {
    return nullptr;
}

State* PlayerIsHit::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile: {
            auto* projectile = dynamic_cast<Projectile*>(collidee);
            if (!projectile->IsPlayerOwned()) {
                // SetDamage(projectile->GetDamage());
                // ApplyDamage();
            }
            break;
        }
        case ObjectType::Enemy:
            GetPlayer()->UnCollide(collidee);
            break;
        case ObjectType::Collider:
            GetPlayer()->UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void PlayerIsHit::PollInput(float dt) {
    MovePlayer(GetPlayer(), dt);
}

void PlayerIsHit::ApplyDamage() {
    int const armor_percentage = GetPlayer()->GetStats().GetArmorPercentage();
    float const damage_multiplier =
        1.0F - (static_cast<float>(armor_percentage) / 100.0F);
    int const modified_damage = m_Damage * damage_multiplier;

    GetPlayer()->GetHealth()->SetDamage(modified_damage);
}

void PlayerDead::Enter() {
    GetPlayer()->GetAnimation()->SelectAnimation("PlayerDead");
}

void PlayerDead::Exit() {}

State* PlayerDead::Update(float /*dt*/) {
    if (GetPlayer()->GetAnimation()->Ended()) {
        GetPlayer()->MarkForDeletion();
    }
    return nullptr;
}

void PlayerDead::Draw() {
    GetPlayer()->DrawAnimation();
}

State* PlayerDead::HandleEvent(Event* /*event*/) {
    return nullptr;
}
