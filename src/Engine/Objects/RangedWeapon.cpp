#include "RangedWeapon.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Renderer/Renderer.h"

RangedWeapon::RangedWeapon(Properties& props, bool playerOwned)
    : Weapon(props, playerOwned) {}

void RangedWeapon::Draw() {
    GameObject::Draw();
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
        SDL_GetTicks() - m_last_fired > m_fire_rate) {
        SDL_Log("weapon rotation: %f", GetRotation());
        SDL_Log("weapon radians: %f", GetRadians());
        m_last_fired = SDL_GetTicks();
        Properties projectile_props("projectile", {0, 0, 723, 724},
                                    {GetMidPointX(), GetMidPointY(), 10, 10},
                                    GetRadians(), "bullet");
        Projectile* projectile = nullptr;
        projectile =
            new Projectile(projectile_props, 10, GetRotation(), PlayerOwned());
        m_ProjectileManager.AddProjectile(projectile);
        ColliderHandler::GetInstance()->AddCollider(projectile);
        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    }
    m_ProjectileManager.UpdateProjectiles(dt);
}

void RangedWeapon::Clean() {}
