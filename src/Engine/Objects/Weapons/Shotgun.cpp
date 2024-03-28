#include "Shotgun.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Projectiles/ProjectileManager.h"
#include "Weapon.h"

const AnimationInfo kDefaultHitAnimationInfo = {"enemy-bullet-explosion",
                                                {0, 0, 16, 16},
                                                4,
                                                150};

void Shotgun::Update(float dt) {
    m_collision_box.Set(GetX(), GetY(), GetHeight(), GetWidth());
    if (InputChecker::IsKeyPressed(SDLK_f)) {
        m_auto_fire_enabled = !m_auto_fire_enabled;
        InputChecker::SetKeyPressed(SDLK_f, false);
    }
    Properties const props = {
        "weapons", {5, 3, 16, 16}, {GetX(), GetY(), 16, 16}};

    const Player* player = Application::Get()->GetPlayer();

    if ((InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT) ||
         m_auto_fire_enabled) &&
        SDL_GetTicks() - m_last_fired > m_stats.GetFireRate()) {
        SDL_Log("getmousex: %f, getmousey: %d",
                static_cast<float>(InputChecker::GetMouseX()),
                InputChecker::GetMouseY());
        SDL_Log("playerx: %f, playery: %f", player->GetMidPointX(),
                player->GetMidPointY());
        m_attack.Shoot(RangedAttackInfo{
            player->GetMidPointX(), player->GetMidPointY(),
            static_cast<float>(InputChecker::GetMouseX()) +
                Renderer::GetInstance()->GetCameraX(),
            static_cast<float>(InputChecker::GetMouseY()) +
                Renderer::GetInstance()->GetCameraY(),
            props, kDefaultHitAnimationInfo, 20, 9, .31459 * 3, true});
        m_last_fired = SDL_GetTicks();
    }

    m_attack.Update(dt);
}
