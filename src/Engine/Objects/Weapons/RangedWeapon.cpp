#include "RangedWeapon.h"
#include "Engine/Objects/ColliderHandler.h"

RangedWeapon::RangedWeapon(Properties& props, RangedWeaponStats& stats)
    : Weapon(props, stats), m_stats(stats) {
    m_projectile_props = new Properties(
        "projectile", {0, 0, 723, 724},
        {GetMidPointX(), GetMidPointY(), 10, 10}, GetRotation(), "bullet");
}

void RangedWeapon::Draw() {
    Weapon::Draw();
    m_ProjectileManager.Draw();
}

void RangedWeapon::Update(float dt) {
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
    SetFlip(SDL_FLIP_NONE);
    if (InputChecker::IsKeyPressed(SDLK_f)) {
        m_auto_fire_enabled = !m_auto_fire_enabled;
        InputChecker::SetKeyPressed(SDLK_f, false);
    }

    if ((InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT) ||
         m_auto_fire_enabled) &&
        SDL_GetTicks() - m_last_fired > m_stats.GetFireRate()) {
        m_last_fired = SDL_GetTicks();

        m_projectile_props->DstRect.x = GetMidPointX();
        m_projectile_props->DstRect.y = GetMidPointY();
        m_projectile_props->Rotation = GetRadians();

        auto* projectile =
            new Projectile(*m_projectile_props,
                           static_cast<float>(m_stats.GetProjectileSpeed()),
                           GetRadians(), PlayerOwned());
        m_ProjectileManager.AddProjectile(projectile);
        ColliderHandler::GetInstance()->AddCollider(projectile);
        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    }
    m_ProjectileManager.UpdateProjectiles(dt);
}

void RangedWeapon::Clean() {}

RangedWeapon::~RangedWeapon() {
    delete m_projectile_props;
}
