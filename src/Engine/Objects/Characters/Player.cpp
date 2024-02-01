#include "Player.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/Weapons/MeleeWeapon.h"
#include "Engine/Objects/Weapons/RangedWeapon.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/State/State.h"

void PlayerStats::Init() {
    Speed = 1.3;
    DodgeCD = 0;
    DodgeSpeed = 1.3;
}

Player::Player(Properties& props) : Character(props) {
    AddStillFrame("face-down", {1, 0, 18, 16});
    AddStillFrame("face-right", {2, 0, 18, 16});
    AddStillFrame("face-right-up", {3, 0, 18, 16});
    AddStillFrame("face-up", {4, 0, 18, 16});

    m_Animation = new Animation();
    m_Animation->AddAnimation("Dead", {m_TextureID, {0, 0, 18, 16}, 6, 5});

    m_Animation->AddAnimation(
        "move-right",
        {m_TextureID, m_StillFrames["face-right"], 6, 5, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation("move-left",
                              {m_TextureID, m_StillFrames["face-right"], 6, 5,
                               SDL_FLIP_HORIZONTAL, true});
    m_Animation->AddAnimation(
        "move-right-up", {m_TextureID, m_StillFrames["face-right-up"], 6, 5,
                          SDL_FLIP_NONE, true});

    m_Animation->AddAnimation(
        "move-left-up", {m_TextureID, m_StillFrames["face-right-up"], 6, 5,
                         SDL_FLIP_HORIZONTAL, true});
    m_Animation->AddAnimation("move-up", {m_TextureID, m_StillFrames["face-up"],
                                          6, 5, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "move-down",
        {m_TextureID, m_StillFrames["face-down"], 6, 5, SDL_FLIP_NONE});

    m_Animation->AddAnimation(
        "dodge-down", {"player-dodge", {0, 0, 20, 20}, 6, 2, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "dodge-right", {"player-dodge", {1, 0, 20, 20}, 6, 2, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "dodge-right-up",
        {"player-dodge", {2, 0, 20, 20}, 6, 2, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "dodge-up", {"player-dodge", {3, 0, 20, 20}, 6, 2, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "dodge-left-up", {"player-dodge", {4, 0, 20, 20}, 6, 2, SDL_FLIP_NONE});
    m_Animation->AddAnimation(
        "dodge-left", {"player-dodge", {5, 0, 20, 20}, 6, 2, SDL_FLIP_NONE});

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

    m_CurrentTilePos = m_StillFrames["face-down"];

    m_Stats.Init();

    ChangeState(new PlayerIdle(this));
    // m_Collider->SetCorrection(-45, -20, 60, 80 )
    m_Health = new Health(100);

    Properties props_g("gun", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    RangedWeaponStats stats_g = {true, 200, 10};
    Weapon* w1 = new RangedWeapon(props_g, stats_g);
    m_Weapons.push_back(w1);

    Properties props_m("weapons", {4, 6, 16, 16}, {0, 0, 16, 20}, 0.0);
    MeleeWeaponStats stats_m = {true, 200, 10};
    Weapon* w2 = new MeleeWeapon(props_m, stats_m);
    w2->SetRotation(50);
    m_Weapons.push_back(w2);

    ColliderHandler::GetInstance()->AddCollider(w2);

    m_CurrentWeapon = w1;
}

Player::~Player() {
    delete m_Health;
    delete m_CurrentState;
    for (auto& weapon : m_Weapons) {
        delete weapon;
    }
}

void Player::Draw() {
    m_CurrentState->Draw();

    m_Health->Draw(GetX(), GetY(), GetWidth());
    m_CurrentWeapon->Draw();
}

void Player::Update(float dt) {
    State* state = m_CurrentState->Update(dt);
    if (state != nullptr) {
        ChangeState(state);
    }

    m_Animation->Update();
    m_RigidBody->Update(dt);

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
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
    float angle = atan2(delta_y, delta_x);
    if (angle < 0) {
        angle += 2 * M_PI;
    } else if (angle > 2 * M_PI) {
        angle -= 2 * M_PI;
    }

    int const weapon_x = GetX() + GetWidth() / 2;
    int const weapon_y = GetY() + GetHeight() / 3;
    m_CurrentWeapon->SetX(weapon_x);
    m_CurrentWeapon->SetY(weapon_y);
    m_CurrentWeapon->SetRotation(angle * (180 / M_PI));
    m_CurrentWeapon->SetFlip(m_Flip);
    m_CurrentWeapon->Update(dt);
}

void Player::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    CollideEvent event(collidee);
    State* state = m_CurrentState->HandleEvent(&event);

    if (state != nullptr) {
        ChangeState(state);
    }
}

void Player::HandleEvent(Event* event) {
    State* state = m_CurrentState->HandleEvent(event);
    if (state != nullptr) {
        ChangeState(state);
    }
}

void Player::Clean() {}
