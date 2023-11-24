#include "Player.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/MeleeWeapon.h"
#include "Engine/Objects/RangedWeapon.h"
#include "Engine/Renderer/Renderer.h"

#include "Projectile.h"

Player::Player(Properties& props) : Character(props) {
    m_Animation = new Animation();
    m_Animation->AddAnimation(
        "Idle", {m_TextureID, m_TilePos, 2, 20, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation("Dead", {"player_dead", {0, 0, 18, 18}, 6, 8});
    m_Animation->AddAnimation(
        "move-right",
        {"player_move_right", {0, 0, 18, 18}, 6, 8, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "move-left",
        {"player_move_right", {0, 0, 18, 18}, 6, 8, SDL_FLIP_HORIZONTAL, true});
    m_Animation->AddAnimation(
        "move-right-up",
        {"player_move_right2", {0, 0, 18, 18}, 6, 8, SDL_FLIP_NONE, true});

    m_Animation->AddAnimation("move-left-up", {"player_move_right2",
                                               {0, 0, 18, 18},
                                               6,
                                               8,
                                               SDL_FLIP_HORIZONTAL,
                                               true});
    m_Animation->AddAnimation(
        "move-up",
        {"player_move_up", {0, 0, 18, 18}, 6, 8, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation(
        "move-down",
        {"player_move_down", {0, 0, 18, 18}, 6, 8, SDL_FLIP_NONE, true});

    m_Animation->SelectAnimation("move-down");
    m_Animation->StopAnimation();
    // m_Collider->SetCorrection(-45, -20, 60, 80 )
    m_Health = new Health(100);

    Properties propsG("gun", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    Weapon* w1 = new RangedWeapon(propsG, true);
    m_Weapons.push_back(w1);

    Properties propsM("melee", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    Weapon* w2 = new MeleeWeapon(propsM, true);
    m_Weapons.push_back(w2);

    //ColliderHandler::GetInstance()->AddCollider(w2);

    m_CurrentWeapon = w1;
    m_MarkedForDeletion = false;
}

Player::~Player() {
    delete m_Health;
    for (auto& weapon : m_Weapons) {
        delete weapon;
    }
}

void Player::Draw() {
    m_Animation->Draw({GetX(), GetY(), GetWidth(), GetHeight()});
    m_Health->Draw(GetX(), GetY(), GetWidth());
    m_CurrentWeapon->Draw();
}

void Player::Update(float dt) {
    if (m_State.HasState(CharacterState::Dead) && m_Animation->Stopped()) {
        m_State.SetState(CharacterState::ToBeDestroyed);
    }
    m_Animation->Update();
    m_RigidBody->Update(dt);

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);

    CheckInput();
    // m_RigidBody.Update(dt);
    // float multiplier = 1;
    // m_RigidBody.UnSetForce();
    // if (InputChecker::IsKeyPressed(SDLK_q) || m_is_dashing) {
    //     m_is_dashing = true;
    //     if (m_able_to_dash % 50 == 0 || m_able_to_dash > 50) {
    //       multiplier = 2;
    //     }
    //     if (m_able_to_dash == 75) {
    //       m_able_to_dash = 0;
    //       m_is_dashing = false;
    //     }
    //     m_able_to_dash++;
    // }

    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());

    UpdateWeapon(dt);
}

void Player::UpdateWeapon(float dt) {
    if (InputChecker::GetMouseWheelDirection() != 0) {
        auto it =
            std::find(m_Weapons.begin(), m_Weapons.end(), m_CurrentWeapon);
        it++;
        if (it == m_Weapons.end()) {
            it = m_Weapons.begin();
        }
        m_CurrentWeapon = *it;

        // Reset the mouse wheel direction to avoid toggling multiple times
        InputChecker::SetMouseWheelDirection(0);
    }

    int weaponXX = GetMidPointX() - Renderer::GetInstance()->GetCameraX();
    int weaponYY = GetMidPointY() - Renderer::GetInstance()->GetCameraY();

    float const delta_x = InputChecker::GetMouseX() - weaponXX;
    float const delta_y = InputChecker::GetMouseY() - weaponYY;
    float angle = atan2(delta_y, delta_x) * (180.0 / M_PI);
    if (angle < 0)
        angle += 360.0F;

    int weaponX = GetX() + GetWidth() / 2;
    int weaponY = GetY() + GetHeight() / 3;
    m_CurrentWeapon->SetX(weaponX);
    m_CurrentWeapon->SetY(weaponY);
    m_CurrentWeapon->SetRotation(angle);
    m_CurrentWeapon->SetFlip(m_Flip);
    m_CurrentWeapon->Update(dt);
}

void Player::ChangeAnimation() {
    SDL_Log("Change animation is being called");
    if (m_State.HasState(CharacterState::Dead)) {
        m_Animation->SelectAnimation("Dead");
        return;
    }

    bool idleY = m_State.HasState(CharacterState::MoveUp) &&
                 m_State.HasState(CharacterState::MoveDown);
    bool idleX = m_State.HasState(CharacterState::MoveLeft) &&
                 m_State.HasState(CharacterState::MoveRight);
    bool movingUp = !idleY && m_State.HasState(CharacterState::MoveUp);
    bool movingDown = !idleY && m_State.HasState(CharacterState::MoveDown);
    bool movingLeft = !idleX && m_State.HasState(CharacterState::MoveLeft);
    bool movingRight = !idleX && m_State.HasState(CharacterState::MoveRight);

    SDL_Log("up: %d, down: %d, left: %d, right: %d", movingUp, movingDown,
            movingLeft, movingRight);

    if (movingUp && movingRight) {
        m_Animation->SelectAnimation("move-right-up");
    } else if (movingUp && movingLeft) {
        m_Animation->SelectAnimation("move-left-up");
    } else if (movingUp) {
        m_Animation->SelectAnimation("move-up");
    } else if (movingRight) {
        m_Animation->SelectAnimation("move-right");
    } else if (movingLeft) {
        m_Animation->SelectAnimation("move-left");
    } else if (movingDown) {
        m_Animation->SelectAnimation("move-down");
    } else {
        m_Animation->StopAnimation();
    }
}

void Player::OnCollide(Collider* collidee) {
    if (this == collidee)
        return;

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            SDL_Log("%s object collided with player", GetID().c_str());
            break;
        case ObjectType::Enemy:
            UnCollide(collidee);
            m_Health->SetDamage(1);
            if (!m_State.HasState(CharacterState::Dead) &&
                m_Health->GetHP() <= 0) {
                m_State.SetState(CharacterState::Dead);
                ChangeAnimation();
            }
            break;
        case ObjectType::MeleeWeapon:
            UnCollide(collidee);
            break;
        case ObjectType::Projectile:
            SDL_Log("%s object collided with projectile", GetID().c_str());
            break;
        case ObjectType::Collider:
            UnCollide(collidee);
            break;
        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
            break;
    }
}

void Player::CheckInput() {
    if (InputChecker::IsKeyPressed(SDLK_q) || m_is_dashing) {
        m_is_dashing = true;
        if (m_able_to_dash % 50 == 0 || m_able_to_dash > 50) {
            m_multiplier = 2;
        }
        if (m_able_to_dash == 75) {
            m_able_to_dash = 0;
            m_is_dashing = false;
        }
        m_able_to_dash++;
    }

    if (InputChecker::IsKeyPressed(SDLK_w)) {
        m_RigidBody->ApplyVelocity(Vector2D(0, -2) * m_multiplier);
    }
    if (InputChecker::IsKeyPressed(SDLK_s)) {
        m_RigidBody->ApplyVelocity(Vector2D(0, 2) * m_multiplier);
    }
    if (InputChecker::IsKeyPressed(SDLK_a)) {
        m_RigidBody->ApplyVelocity(Vector2D(-2, 0) * m_multiplier);
        SetFlip(SDL_FLIP_HORIZONTAL);
    }
    if (InputChecker::IsKeyPressed(SDLK_d)) {
        m_RigidBody->ApplyVelocity(Vector2D(2, 0) * m_multiplier);
        SetFlip(SDL_FLIP_NONE);
    }
}

void Player::OnKeyPressed(SDL_Event& event) {
    if (event.key.repeat != 0)
        return;
    if (event.key.keysym.sym == SDLK_w) {
        m_State.AddState(CharacterState::MoveUp);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_s) {
        m_State.AddState(CharacterState::MoveDown);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_a) {
        m_State.AddState(CharacterState::MoveLeft);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_d) {
        m_State.AddState(CharacterState::MoveRight);
        ChangeAnimation();
    }
}

void Player::OnKeyReleased(SDL_Event& event) {
    //Need a better way to do this... this does not work if outside forces affect velocity

    if (event.key.keysym.sym == SDLK_w) {
        Vector2D adjust = Vector2D(0, 3);
        //adjust.Y = m_RigidBody->Velocity().Y != 0 ? adjust.Y : 0;
        m_State.RemoveState(CharacterState::MoveUp);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_s) {
        Vector2D adjust = Vector2D(0, -3);
        //adjust.Y = m_RigidBody->Velocity().Y != 0 ? adjust.Y : 0;
        m_State.RemoveState(CharacterState::MoveDown);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_a) {
        Vector2D adjust = Vector2D(3, 0);
        //adjust.X = m_RigidBody->Velocity().X != 0 ? adjust.X : 0;
        m_State.RemoveState(CharacterState::MoveLeft);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_d) {
        Vector2D adjust = Vector2D(-3, 0);
        //adjust.X = m_RigidBody->Velocity().X != 0 ? adjust.X : 0;
        m_State.RemoveState(CharacterState::MoveRight);
        ChangeAnimation();
    }
}

void Player::Clean() {}
