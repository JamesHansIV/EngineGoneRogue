#include "Weapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Objects/ColliderHandler.h"

std::vector<Projectile*> projectiles;

Weapon::Weapon(Properties& props, WeaponType weaponType) : Collider(props), m_Type(weaponType) {}

void Weapon::Draw()
{
    GameObject::Draw();
    for (auto *projectile : projectiles) {
        projectile->Draw();
    }
}

void Weapon::Update(float dt)
{
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
    if (m_Type == PROJECTILE)
    {
        SetFlip(SDL_FLIP_NONE);
        Properties projectile_props("projectile", {0, 0, 723, 724}, {GetMidPointX(), GetMidPointY(), 10, 10}, GetRotation());
        if (InputChecker::IsKeyPressed(SDLK_f)) {
          m_auto_fire_enabled = !m_auto_fire_enabled;
          InputChecker::SetKeyPressed(SDLK_f, false);
        }
        if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT) || m_auto_fire_enabled)
        {
            Projectile* projectile = nullptr;
            projectile = new Projectile(projectile_props, 50, 1.0, GetRotation());
            projectiles.push_back(projectile);
            InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
        }
        for (auto it = projectiles.begin(); it != projectiles.end();)
        {
            (*it)->Update(dt, m_Colliders);
            if ((*it)->IsMarkedForDeletion())
            {
                (*it)->Clean();
                delete *it;
                it = projectiles.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    else if(m_Type == MELEE)
    {
        if(GetFlip() == SDL_FLIP_HORIZONTAL)
        {
            SetX(GetX() - 12);
            m_CollisionBox.Set(GetX()-12, GetY()-12, GetHeight(), GetWidth());
        }

        if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            float swing_angle = 0.0F;
            if(GetFlip() == SDL_FLIP_HORIZONTAL)
            {
                swing_angle = -45.0F;
            }
            else
            {
                swing_angle = 45.0F;
            }
            SetRotation(swing_angle);
            for (auto *collider : m_Colliders)
            {
                if(ColliderHandler::GetInstance()->CheckCollision(m_CollisionBox.GetRect(), collider->GetCollisionBox().GetRect()))
                {
                    collider->GetHealthObj()->SetDamage(10);
                }
            }
            InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
        }
        else
        {
            SetRotation(0.0);
        }
    }
}

void Weapon::Clean()
{

}
