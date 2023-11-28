#include "Player.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Entrance.h"
#include "Engine/Objects/MeleeWeapon.h"
#include "Engine/Objects/RangedWeapon.h"
#include "Engine/Renderer/Renderer.h"

#include "Projectile.h"

Player::Player(Properties& props) : Character(props) {
    AddIdleFrame("face-down", {1, 0, 18, 16});
    AddIdleFrame("face-right", {2, 0, 18, 16});
    AddIdleFrame("face-right-up", {3, 0, 18, 16});
    AddIdleFrame("face-up", {4, 0, 18, 16});

    m_Animation = new Animation();
    m_Animation->AddAnimation("Dead", {m_TextureID, {0, 0, 18, 16}, 6, 5});
    m_Animation->AddAnimation(
        "move-right",
        {m_TextureID, m_IdleFrames["face-right"], 6, 5, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation("move-left",
                              {m_TextureID, m_IdleFrames["face-right"], 6, 5,
                               SDL_FLIP_HORIZONTAL, true});
    m_Animation->AddAnimation("move-right-up",
                              {m_TextureID, m_IdleFrames["face-right-up"], 6, 5,
                               SDL_FLIP_NONE, true});

    m_Animation->AddAnimation("move-left-up",
                              {m_TextureID, m_IdleFrames["face-right-up"], 6, 5,
                               SDL_FLIP_HORIZONTAL, true});
    m_Animation->AddAnimation(
        "move-up", {m_TextureID, m_IdleFrames["face-up"], 6, 5, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "move-down",
        {m_TextureID, m_IdleFrames["face-down"], 6, 5, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "front-hit", {m_TextureID, {5, 0, 18, 16}, 2, 20, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "right-hit", {m_TextureID, {5, 2, 18, 16}, 2, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "left-hit", {m_TextureID, {5, 2, 18, 16}, 2, 8, SDL_FLIP_HORIZONTAL});
    m_Animation->AddAnimation(
        "right-up-hit", {m_TextureID, {5, 4, 18, 16}, 2, 8, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "left-up-hit",
        {m_TextureID, {5, 4, 18, 16}, 2, 8, SDL_FLIP_HORIZONTAL});
    m_Animation->AddAnimation(
        "back-hit", {m_TextureID, {6, 0, 18, 16}, 2, 8, SDL_FLIP_NONE});

    m_CurrentTilePos = m_IdleFrames["face-down"];

    m_State.AddState(CharacterState::Idle);
    // m_Collider->SetCorrection(-45, -20, 60, 80 )
    m_Health = new Health(100);

    Properties props_g("gun", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    Weapon* w1 = new RangedWeapon(props_g, true);
    m_Weapons.push_back(w1);

    Properties props_m("melee", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    Weapon* w2 = new MeleeWeapon(props_m, true);
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
    if (m_State.HasState(CharacterState::Idle) &&
        !m_State.HasState(CharacterState::IsHit)) {
        GameObject::DrawRect();
    } else {
        m_Animation->Draw({GetX(), GetY(), GetWidth(), GetHeight()});
    }
    m_Health->Draw(GetX(), GetY(), GetWidth());
    m_CurrentWeapon->Draw();
}

void Player::Update(float dt) {
    if (m_State.HasState(CharacterState::Dead) && m_Animation->Ended()) {
        m_State.SetState(CharacterState::ToBeDestroyed);
    }
    if (m_State.HasState(CharacterState::IsHit) && m_Animation->Ended()) {
        m_State.RemoveState(CharacterState::IsHit);
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

    int const weapon_xx =
        GetMidPointX() - Renderer::GetInstance()->GetCameraX();
    int const weapon_yy =
        GetMidPointY() - Renderer::GetInstance()->GetCameraY();

    float const delta_x = InputChecker::GetMouseX() - weapon_xx;
    float const delta_y = InputChecker::GetMouseY() - weapon_yy;
    float angle = atan2(delta_y, delta_x) * (180.0 / M_PI);
    if (angle < 0) {
        angle += 360.0F;
    } else if (angle > 360) {
        angle -= 360.0F;
    }

    int const weapon_x = GetX() + GetWidth() / 2;
    int const weapon_y = GetY() + GetHeight() / 3;
    m_CurrentWeapon->SetX(weapon_x);
    m_CurrentWeapon->SetY(weapon_y);
    m_CurrentWeapon->SetRotation(angle);
    m_CurrentWeapon->SetFlip(m_Flip);
    m_CurrentWeapon->Update(dt);
}

void Player::ChangeAnimation() {
    // SDL_Log("Change animation is being called");
    if (m_State.HasState(CharacterState::Dead)) {
        m_Animation->SelectAnimation("Dead");
        return;
    }
    if (m_State.HasState(CharacterState::IsHit)) {
        return;
    }

    bool const idle_y = m_State.HasState(CharacterState::MoveUp) &&
                        m_State.HasState(CharacterState::MoveDown);
    bool const idle_x = m_State.HasState(CharacterState::MoveLeft) &&
                        m_State.HasState(CharacterState::MoveRight);
    bool const moving_up = !idle_y && m_State.HasState(CharacterState::MoveUp);
    bool const moving_down =
        !idle_y && m_State.HasState(CharacterState::MoveDown);
    bool const moving_left =
        !idle_x && m_State.HasState(CharacterState::MoveLeft);
    bool const moving_right =
        !idle_x && m_State.HasState(CharacterState::MoveRight);

    // SDL_Log("up: %d, down: %d, left: %d, right: %d", movingUp, movingDown, movingLeft, movingRight);
    if (moving_up || moving_down || moving_left || moving_right) {
        m_State.RemoveState(CharacterState::Idle);
    } else {
        m_State.AddState(CharacterState::Idle);
    }

    if (moving_up && moving_right) {
        m_Animation->SelectAnimation("move-right-up");
        m_CurrentTilePos = m_IdleFrames["face-right-up"];
        SetFlip(SDL_FLIP_NONE);
    } else if (moving_up && moving_left) {
        m_Animation->SelectAnimation("move-left-up");
        m_CurrentTilePos = m_IdleFrames["face-right-up"];
        SetFlip(SDL_FLIP_HORIZONTAL);
    } else if (moving_up) {
        m_Animation->SelectAnimation("move-up");
        m_CurrentTilePos = m_IdleFrames["face-up"];
        SetFlip(SDL_FLIP_NONE);
    } else if (moving_right) {
        m_Animation->SelectAnimation("move-right");
        m_CurrentTilePos = m_IdleFrames["face-right"];
        SetFlip(SDL_FLIP_NONE);
    } else if (moving_left) {
        m_Animation->SelectAnimation("move-left");
        m_CurrentTilePos = m_IdleFrames["face-right"];
        SetFlip(SDL_FLIP_HORIZONTAL);
    } else if (moving_down) {
        m_Animation->SelectAnimation("move-down");
        m_CurrentTilePos = m_IdleFrames["face-down"];
        SetFlip(SDL_FLIP_NONE);
    } else {
        m_Animation->StopAnimation();
    }
}

void Player::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            break;
        case ObjectType::Enemy: {
            //UnCollide(collidee);
            SDL_Log("player oncollide with enemy");
            int const frame = collidee->GetAnimation()->GetCurrentFrame();
            if (collidee->GetState().HasState(CharacterState::Attack) &&
                2 <= frame && frame <= 4) {
                //m_Health->SetDamage(1);
                if (!m_State.HasState(CharacterState::Dead) &&
                    !m_State.HasState(CharacterState::IsHit)) {
                    m_State.AddState(CharacterState::IsHit);
                    m_Animation->SelectAnimation("front-hit");
                }
            }
            if (!m_State.HasState(CharacterState::Dead) &&
                m_Health->GetHP() <= 0) {
                m_State.SetState(CharacterState::Dead);
                ChangeAnimation();
            }

            break;
        }
        case ObjectType::MeleeWeapon:
            //UnCollide(collidee);
            break;
        case ObjectType::Projectile:
            if (dynamic_cast<Projectile*>(collidee)->PlayerOwned()) {
                break;
            }
            if (!m_State.HasState(CharacterState::Dead) &&
                !m_State.HasState(CharacterState::IsHit)) {
                m_State.AddState(CharacterState::IsHit);
                m_Animation->SelectAnimation("front-hit");
            }
            if (!m_State.HasState(CharacterState::Dead) &&
                m_Health->GetHP() <= 0) {
                m_State.SetState(CharacterState::Dead);
                ChangeAnimation();
            }
            break;
        case ObjectType::Entrance: {
            auto* entrance = dynamic_cast<Entrance*>(collidee);
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

void Player::CheckInput() {
    if (InputChecker::IsKeyPressed(SDLK_q) || m_is_dashing) {
        m_is_dashing = true;
        if (m_able_to_dash % 50 == 0 || m_able_to_dash > 50) {
            m_multiplier = 2;
        }
        if (m_able_to_dash == 75) {
            m_able_to_dash = 0;
            m_is_dashing = false;
            m_multiplier = 1;
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
    if (event.key.repeat != 0) {
        return;
    }
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

    if (event.key.keysym.sym == SDLK_w) {
        m_State.RemoveState(CharacterState::MoveUp);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_s) {
        m_State.RemoveState(CharacterState::MoveDown);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_a) {
        m_State.RemoveState(CharacterState::MoveLeft);
        ChangeAnimation();
    } else if (event.key.keysym.sym == SDLK_d) {
        m_State.RemoveState(CharacterState::MoveRight);
        ChangeAnimation();
    }
}

void Player::Clean() {}
