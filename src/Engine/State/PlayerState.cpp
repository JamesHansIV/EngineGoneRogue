#include "PlayerState.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Enemy.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Objects/Player.h"
#include "SDL2/SDL_events.h"

bool IsPlayerDead(Player* player) {
    return (player->GetHealth()->GetHP() < 0);
}

State* updateDirection(Player* player) {
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
        player->GetAnimation()->SelectAnimation("move-right-up");
        player->SelectStillFrame("face-right-up");
        player->SetFlip(SDL_FLIP_NONE);
    } else if (moving_up && moving_left) {
        player->GetAnimation()->SelectAnimation("move-left-up");
        player->SelectStillFrame("face-right-up");
        player->SetFlip(SDL_FLIP_HORIZONTAL);
    } else if (moving_up) {
        player->GetAnimation()->SelectAnimation("move-up");
        player->SelectStillFrame("face-up");
        player->SetFlip(SDL_FLIP_NONE);
    } else if (moving_right) {
        player->GetAnimation()->SelectAnimation("move-right");
        player->SelectStillFrame("face-right");
        player->SetFlip(SDL_FLIP_NONE);
    } else if (moving_left) {
        player->GetAnimation()->SelectAnimation("move-left");
        player->SelectStillFrame("face-right");
        player->SetFlip(SDL_FLIP_HORIZONTAL);
    } else if (moving_down) {
        player->GetAnimation()->SelectAnimation("move-down");
        player->SelectStillFrame("face-down");
        player->SetFlip(SDL_FLIP_NONE);
    } else {
        player->GetAnimation()->StopAnimation();
    }
    return nullptr;
}

State* handleEnemyCollide(Player* player, Enemy* enemy) {
    player->UnCollide(enemy);
    int const frame = enemy->GetAnimation()->GetCurrentFrame();
    if (enemy->GetCurrentState()->GetType() == StateType::Attack &&
        2 <= frame && frame <= 4) {
        return new PlayerIsHit(player, 1);
    }
    return nullptr;
}

State* handleProjectileCollide(Player* player, Projectile* projectile) {
    if (projectile->PlayerOwned()) {
        return nullptr;
    }
    return new PlayerIsHit(player, 10);
}

State* handleEntranceCollide(Player* player, Entrance* entrance) {
    if (!entrance->GetState().HasState(EntranceState::Open)) {
        player->UnCollide(entrance);
    }
    return nullptr;
}

State* handleCollide(Player* player, CollideEvent* event) {

    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Enemy: {
            return handleEnemyCollide(player, dynamic_cast<Enemy*>(collidee));
            break;
        }
        case ObjectType::Projectile:
            return handleProjectileCollide(player,
                                           dynamic_cast<Projectile*>(collidee));
            break;
        case ObjectType::Entrance: {
            return handleEntranceCollide(player,
                                         dynamic_cast<Entrance*>(collidee));
            break;
        }
        case ObjectType::Collider:
            player->UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void PlayerIdle::Enter() {}

void PlayerIdle::Exit() {}

State* PlayerIdle::Update(float dt) {
    return nullptr;
}

void PlayerIdle::Draw() {
    GetPlayer()->DrawRect();
}

State* PlayerIdle::HandleEvent(Event* event) {
    EventType e_type = event->GetEventType();

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
    return handleCollide(GetPlayer(), event);
}

void PlayerMoving::Enter() {
    updateDirection(GetPlayer());
}

void PlayerMoving::Exit() {}

State* PlayerMoving::Update(float dt) {
    return nullptr;
}

void PlayerMoving::Draw() {
    GetPlayer()->DrawAnimation();
}

State* PlayerMoving::HandleEvent(Event* event) {
    EventType e_type = event->GetEventType();

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
            return updateDirection(GetPlayer());
            break;
        default:
            break;
    }
    return nullptr;
}

State* PlayerMoving::OnCollideEvent(CollideEvent* event) {
    return handleCollide(GetPlayer(), event);
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
    return nullptr;
}

void PlayerIsHit::Draw() {
    GetPlayer()->DrawAnimation();
}

State* PlayerIsHit::HandleEvent(Event* event) {
    EventType e_type = event->GetEventType();

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

State* PlayerIsHit::OnUserEvent(UserEvent* event) {
    return nullptr;
}

State* PlayerIsHit::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Projectile:
            if (!dynamic_cast<Projectile*>(collidee)->PlayerOwned()) {
                SetDamage(10);
                ApplyDamage();
            }
            break;
        case ObjectType::Enemy:
            GetPlayer()->UnCollide(collidee);
            SetDamage(1);
            ApplyDamage();
            break;
        case ObjectType::Collider:
            GetPlayer()->UnCollide(collidee);
            break;
        default:
            break;
    }
    return nullptr;
}

void PlayerIsHit::ApplyDamage() {
    GetPlayer()->GetHealth()->SetDamage(m_Damage);
}

void PlayerDead::Enter() {
    SDL_Log("enter dead state");
    GetPlayer()->GetAnimation()->SelectAnimation("PlayerDead");
}

void PlayerDead::Exit() {}

State* PlayerDead::Update(float dt) {
    if (GetPlayer()->GetAnimation()->Ended()) {
        GetPlayer()->MarkForDeletion();
    }
    return nullptr;
}

void PlayerDead::Draw() {
    GetPlayer()->DrawAnimation();
}

State* PlayerDead::HandleEvent(Event* event) {
    return nullptr;
}
