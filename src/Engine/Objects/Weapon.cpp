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
    int const player_x = GetMidPointX() - Renderer::GetInstance()->GetCameraX();
    int const player_y = GetMidPointY() - Renderer::GetInstance()->GetCameraY();
    int const mouse_x = InputChecker::GetMouseX();
    int const mouse_y = InputChecker::GetMouseY();

    // Calculate the angle between the mouse and the player
    float const delta_x = mouse_x - player_x;
    float const delta_y = mouse_y - player_y;
    float angle = atan2(delta_y, delta_x) * (180.0 / M_PI);
    // Convert the angle range from -180 to 180 to 0 to 360
    if (angle < 0) angle += 360.0F;

    SDL_Log("%f", angle);

    if (m_Type == PROJECTILE)
    {
        Properties projectile_props("projectile", {0, 0, 723, 724}, {GetMidPointX(), GetMidPointY(), 10, 10}, angle);
        if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            Projectile* projectile = nullptr;
            projectile = new Projectile(projectile_props, 50, 1.0, angle);
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