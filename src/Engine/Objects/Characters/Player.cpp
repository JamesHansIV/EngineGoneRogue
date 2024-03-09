#include "Player.h"
#include <string>
#include <thread>
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/ItemInventory.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Objects/Weapons/MeleeWeapon.h"
#include "Engine/Objects/Weapons/RangedWeapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/State/State.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"

const int kMoveAnimationSpeed = 20;
const int kIdleAnimationSpeed = 50;
const int kProjectileWidth = 10;
const int kProjectileHeight = 10;
// Experience multiplier for each level
double k_experience_multiplier = 9.0 / 10.0;

Player::Player(Properties& props) : Character(props) {
    Init();
}

Player::Player(Collider* rhs) : Character(rhs) {
    Init();
}

void Player::Init() {
    AddStillFrame("face-down", {1, 0, 18, 16});
    AddStillFrame("face-right", {2, 0, 18, 16});
    AddStillFrame("face-right-up", {3, 0, 18, 16});
    AddStillFrame("face-up", {4, 0, 18, 16});

    m_CurrentTilePos = m_StillFrames["face-down"];

    m_stats = new PlayerStats(MovementInfo{80, 0.75, 100, 80},
                              CombatInfo{1, 1, 0, 50}, HealthInfo{1, 20});
    Properties* default_projectile_props = new Properties(
        "weapons", {6, 0, 16, 16},
        {GetMidPointX(), GetMidPointY(), kProjectileWidth, kProjectileHeight},
        GetRotation(), "bullet");
    Properties* bow_projectile_props =
        new Properties("weapons", {2, 6, 16, 16},
                       {GetMidPointX() * 2, GetMidPointY() * 2,
                        kProjectileWidth * 2, kProjectileHeight * 2},
                       GetRotation(), "arrow");

    ChangeState(new PlayerIdle(this));
    // m_Collider->SetCorrection(-45, -20, 60, 80 )
    m_Health = new Health(100);

    Properties props_uzi("weapons", {0, 3, 16, 16}, {0, 0, 18, 18}, 0.0);
    RangedWeaponStats stats_uzi = {true, 200, 30, 16, m_stats};
    Weapon* uzi = new RangedWeapon(props_uzi, stats_uzi, this, "Uzi",
                                   default_projectile_props);
    m_Weapons.push_back(uzi);

    Properties props_pistol("weapons", {0, 1, 16, 16}, {0, 0, 18, 18}, 0.0);
    RangedWeaponStats stats_pistol = {true, 400, 28, 34, m_stats};
    Weapon* pistol = new RangedWeapon(props_pistol, stats_pistol, this,
                                      "Pistol", default_projectile_props);
    m_Weapons.push_back(pistol);

    Properties props_sniper("sniper", {0, 0, 32, 16}, {0, 0, 36, 16}, 0.0);
    RangedWeaponStats stats_sniper = {true, 1000, 40, 100, m_stats};
    Weapon* sniper =
        new RangedWeapon(props_sniper, stats_sniper, this,
                         std::string("Sniper"), default_projectile_props);
    m_Weapons.push_back(sniper);

    Properties props_m("weapons", {4, 6, 16, 16}, {0, 0, 16, 20}, 0.0);
    MeleeWeaponStats stats_m = {true, 200, 10, 10, m_stats};
    Weapon* w2 = new MeleeWeapon(props_m, stats_m, this, std::string("Sword"));
    w2->SetRotation(50);
    m_Weapons.push_back(w2);

    Properties props_bow("weapons", {2, 5, 16, 16}, {0, 0, 18, 18}, 45.0);
    RangedWeaponStats stats_bow = {true, 750, 35, 75, m_stats};
    Weapon* bow = new RangedWeapon(props_bow, stats_bow, this,
                                   std::string("Bow"), bow_projectile_props);
    m_Weapons.push_back(bow);

    ColliderHandler::GetInstance()->AddCollider(w2);

    m_CurrentWeapon = m_Weapons[0];
}

void Player::Draw() {
    m_CurrentState->Draw();
    m_Health->Draw(GetX(), GetY(), GetWidth());
    m_CurrentWeapon->Draw();
    for (auto& weapon : m_Weapons) {
        if (auto* ranged = dynamic_cast<RangedWeapon*>(weapon)) {
            ranged->DrawProjectiles();
        }
    }
}

void Player::AddItem(Item* item) {
    if (m_Items.size() >= 12) {
        SDL_Log("Inventory is full. Cannot add more items");
        return;
    }
    m_Items[item->GetName()] = item;
    item->AddCount();
}

void Player::Update(float dt) {
    State* state = m_CurrentState->Update(dt);
    if (state != nullptr) {
        ChangeState(state);
    }

    if (SDL_GetTicks() - m_lastHealthRegen > 1000) {
        m_lastHealthRegen = SDL_GetTicks();
        m_Health->IncreaseHealth(m_stats->GetHPRegenRate());
    }
    m_stats->Update();
    m_Animation->Update();
    m_RigidBody->Update(dt);

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
    //SDL_Log("Player position: %f, %f", GetX(), GetY());
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());

    UpdateWeapon(dt);
}

bool IsLookingBehind(float angle) {
    const float upper_bound = 4 * M_PI / 3;
    const float lower_bound = 2 * M_PI / 3;
    return angle < upper_bound && angle > lower_bound;
}

void Player::UpdateWeapon(float dt) {
    if (InputChecker::GetMouseWheelDirection() > 0) {
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

    if (InputChecker::GetMouseWheelDirection() < 0) {
        auto it =
            std::find(m_Weapons.rbegin(), m_Weapons.rend(), m_CurrentWeapon);
        it++;
        if (it == m_Weapons.rend()) {
            it = m_Weapons.rbegin();
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
    for (auto& weapon : m_Weapons) {
        if (auto* ranged = dynamic_cast<RangedWeapon*>(weapon)) {
            ranged->UpdateProjectiles(dt);
        }
    }
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
            m_stats->IncrementKillCount();
            break;
        }
        default:
            if (m_CurrentState != nullptr) {
                state = m_CurrentState->HandleEvent(event);
            }
    }
    if (state != nullptr) {
        ChangeState(state);
    }
}

Player::~Player() {
    delete m_Health;
    delete m_CurrentState;
    delete m_Animation;
    for (auto& weapon : m_Weapons) {
        delete weapon;
    }
    for (auto& item_pair : m_Items) {
        delete item_pair.second;
    }
    m_Items.clear();
}

void PlayerStats::AddExperience(int experience) {
    int experience_to_add = experience;
    int stored_experience = 0;
    int cur_experience = m_experience;

    // Add experience based on cur level
    for (int i = 0; i < m_level; i++) {
        experience_to_add *= k_experience_multiplier;
    }
    // If there is a level-up experience, need to add experience based on new level
    // Repeat until there is no level-up (overflow)
    while (experience_to_add + cur_experience >= 100) {
        const int experience_added_this_level = 100 - cur_experience;
        experience_to_add =
            std::round(k_experience_multiplier *
                       (experience_to_add - experience_added_this_level));
        stored_experience += experience_added_this_level;
        cur_experience = 0;
    }
    SDL_Log("Experience added: %d", experience_to_add + stored_experience);

    m_experience += experience_to_add + stored_experience;
}

void Player::Clean() {}
