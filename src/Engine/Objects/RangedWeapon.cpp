#include "RangedWeapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Objects/ColliderHandler.h"


RangedWeapon::RangedWeapon(Properties& props, bool playerOwned) : Weapon(props, playerOwned) {}

void RangedWeapon::Draw()
{
    GameObject::Draw();
    for (auto *projectile : m_Projectiles) {
        projectile->Draw();
    }
}

void RangedWeapon::Update(float dt)
{
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
    SetFlip(SDL_FLIP_NONE);
    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT))
    {
        Properties projectile_props("projectile", {0, 0, 723, 724}, {GetMidPointX(), GetMidPointY(), 10, 10}, GetRotation(), "bullet");
        Projectile* projectile = nullptr;
        projectile = new Projectile(projectile_props, 10, GetRotation(), GetPlayerOwned());
        m_Projectiles.push_back(projectile);
        ColliderHandler::GetInstance()->AddCollider(projectile);
        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    }
    
    UpdateProjectiles(dt);
}

void RangedWeapon::UpdateProjectiles(float dt) {
    for (auto it = m_Projectiles.begin(); it != m_Projectiles.end();)
    {
        Projectile* projectile = *it;
        (projectile)->Update(dt);
        if ((projectile)->IsMarkedForDeletion())
        {
            ColliderHandler::GetInstance()->RemoveCollider(projectile);
            (projectile)->Clean();
            delete projectile;
            it = m_Projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void RangedWeapon::Clean()
{

}