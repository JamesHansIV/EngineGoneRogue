#include "Player.h"
#include <thread>
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Objects/Weapons/Bow.h"
#include "Engine/Objects/Weapons/MeleeWeapon.h"
#include "Engine/Objects/Weapons/RangedWeapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/State/State.h"
#include "SDL2/SDL_render.h"

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

    m_stats = new PlayerStats(1, 0, 1.3, 1, 1, 1, 50, 1, 20);

    ChangeState(new PlayerIdle(this));
    // m_Collider->SetCorrection(-45, -20, 60, 80 )
    m_Health = new Health(100);

    Properties props_uzi("uzi", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    RangedWeaponStats stats_uzi = {true, 200, 10, 16, m_stats};
    Weapon* uzi = new RangedWeapon(props_uzi, stats_uzi, this);
    m_Weapons.push_back(uzi);

    Properties props_pistol("pistol", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    RangedWeaponStats stats_pistol = {true, 400, 7, 34, m_stats};
    Weapon* pistol = new RangedWeapon(props_pistol, stats_pistol, this);
    m_Weapons.push_back(pistol);

    Properties props_sniper("sniper", {0, 0, 35, 16}, {0, 0, 35, 16}, 0.0);
    RangedWeaponStats stats_sniper = {true, 1000, 10, 100, m_stats};
    Weapon* sniper = new RangedWeapon(props_sniper, stats_sniper, this);
    m_Weapons.push_back(sniper);

    Properties props_m("weapons", {4, 6, 16, 16}, {0, 0, 16, 20}, 0.0);
    MeleeWeaponStats stats_m = {true, 200, 10, 10, m_stats};
    Weapon* w2 = new MeleeWeapon(props_m, stats_m, this);
    w2->SetRotation(50);
    m_Weapons.push_back(w2);
          
    Properties props_bow("bow", {0, 0, 16, 18}, {0, 0, 16, 18}, 90.0);
    RangedWeaponStats stats_bow = {true, 750, 10, 75, m_stats};
    Weapon* bow = new Bow(props_bow, stats_bow, this);
    m_Weapons.push_back(bow);

    ColliderHandler::GetInstance()->AddCollider(w2);

    m_CurrentWeapon = m_Weapons[0];

    std::thread health_regen_thread([this]() {
        while (true) {
            SDL_Log("Cur health: %d", m_Health->GetHP());
            m_Health->IncreaseHealth(m_stats->getHPRegenRate());
            SDL_Log("Health regen done to: %d", m_Health->GetHP());
            // Sleep for one second
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    m_ExperienceBar = new ExperienceBar(100, 0);
    // Detach the health regeneration thread so it can run independently
    health_regen_thread.detach();
}

Player::~Player() {
    delete m_Health;
    delete m_CurrentState;
    delete m_ExperienceBar;
    for (auto& weapon : m_Weapons) {
        delete weapon;
    }
}

void Player::Draw() {
    m_CurrentState->Draw();
    m_ExperienceBar->Draw();

    m_Health->Draw(GetX(), GetY(), GetWidth());
    m_CurrentWeapon->Draw();
}

void Player::Update(float dt) {
    State* state = m_CurrentState->Update(dt);
    if (state != nullptr) {
        ChangeState(state);
    }

    m_ExperienceBar->SetExperience(m_stats->getExperience());

    m_Animation->Update();
    m_RigidBody->Update(dt);

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());

    UpdateWeapon(dt);
}

bool IsLookingBehind(float angle) {
    const float upper_bound = 4 * M_PI / 3;
    const float lower_bound = 2 * M_PI / 3;
    return angle < upper_bound && angle > lower_bound;
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

    SDL_RendererFlip weapon_flip = SDL_FLIP_NONE;
    if (IsLookingBehind(angle)) {
        weapon_flip = SDL_FLIP_VERTICAL;
    }

    double const weapon_x = GetX() + GetWidth() / 2.5;
    double const weapon_y = GetY() + GetHeight() / 2.5;
    m_CurrentWeapon->SetX(weapon_x);
    m_CurrentWeapon->SetY(weapon_y);
    m_CurrentWeapon->SetRotation(angle * (180 / M_PI));
    m_CurrentWeapon->SetFlip(weapon_flip);
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
    EventType const e_type = event->GetEventType();

    State* state = nullptr;

    switch (e_type) {
        case EventType::EnemyDeathEvent: {
            auto* death_event = dynamic_cast<EnemyDeathEvent*>(event);
            m_stats->AddExperience(death_event->GetEnemyStats().xpGiven);
            break;
        }
        default:
            state = m_CurrentState->HandleEvent(event);
    }
    if (state != nullptr) {
        ChangeState(state);
    }
}

void Player::Clean() {}
