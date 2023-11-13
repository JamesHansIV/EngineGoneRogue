#include "Weapon.h"
#include "Engine/Renderer/Renderer.h"

std::vector<Projectile*> projectiles;

void Weapon:: Draw()
{
    if (m_Type == MELEE) {
        SetRotation(0.0);
    }
    GameObject::Draw();
    for (auto *projectile : projectiles) {
        projectile->Draw();
    }
}

void Weapon::Update(float dt)
{
    if (m_Type == PROJECTILE)
    {
        Properties projectile_props("projectile", {0, 0, 723, 724}, {GetMidPointX(), GetMidPointY(), 10, 10}, GetRotation());
        if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT))
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
}

void Weapon::Clean()
{

}