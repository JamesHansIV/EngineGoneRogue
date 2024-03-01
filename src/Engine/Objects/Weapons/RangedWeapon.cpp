#include "RangedWeapon.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/ColliderHandler.h"

RangedWeapon::RangedWeapon(Properties& props, RangedWeaponStats& stats,
                           Player* owner, const std::string& name,
                           Properties* projectile_props)
    : Weapon(props, stats, owner, name),
      m_stats(stats),
      m_projectile_props(projectile_props) {
    m_auto_fire_enabled = true;
}

void RangedWeapon::Draw() {
    Weapon::Draw();
}

void RangedWeapon::DrawProjectiles() {}

void RangedWeapon::Update(float dt) {
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
    if (InputChecker::IsKeyPressed(SDLK_f)) {
        m_auto_fire_enabled = !m_auto_fire_enabled;
        InputChecker::SetKeyPressed(SDLK_f, false);
    }

    if ((InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT) ||
         m_auto_fire_enabled) &&
        SDL_GetTicks() - m_last_fired > m_stats.GetFireRate()) {
        m_last_fired = SDL_GetTicks();

        m_projectile_props->DstRect.x = GetMidPointX();
        m_projectile_props->DstRect.y = GetMidPointY() - 6;
        m_projectile_props->Rotation = GetRotation();

        auto* projectile = new Projectile(
            *m_projectile_props,
            static_cast<float>(m_stats.GetProjectileSpeed()), GetRadians(),
            IsPlayerOwned(),
            m_stats.GetDamage() + m_stats.GetOwnerStats()->GetRangedDamage(),
            m_stats.GetOwnerStats()->GetPiercing(), GetOwner());
        // Add player velocity to projectile. Have to multiply by .1 to make the
        // player velocity to be weaker than the projectile velocity, so that it does
        // not overpower the projectile velocity.
        projectile->SetVelocity(m_owner->GetRigidBody()->Velocity() * .1 +
                                projectile->GetVelocity());
        ProjectileManager::GetInstance()->AddProjectile(projectile);
        ColliderHandler::GetInstance()->AddCollider(projectile);
        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    }
}

void RangedWeapon::UpdateProjectiles(float dt) {}

void RangedWeapon::Clean() {}

RangedWeapon::~RangedWeapon() {
    delete m_projectile_props;
}
