#include "AttackFunctions.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/Projectiles/HelixBullet.h"
#include "Engine/Objects/Projectiles/RotatingBullet.h"

float CalculateAngle(RangedAttackInfo& info) {
    float const delta_y = info.TargetY - info.Y;
    float const delta_x = info.TargetX - info.X;

    float const angle = atan2(delta_y, delta_x);

    info.ProjProps.Rotation = angle * 180 / M_PI;

    return angle;
}

std::vector<Projectile*> CreateBullet(RangedAttackInfo info) {
    std::vector<Projectile*> bullets;

    float const angle = CalculateAngle(info);

    auto* bullet = new Projectile(info.ProjProps, info.ProjSpeed, angle,
                                  info.HitAnimation, info.IsPlayerOwned);

    bullets.push_back(bullet);

    return bullets;
}

std::vector<Projectile*> CreateSpreadBullet(RangedAttackInfo info) {
    std::vector<Projectile*> bullets;

    float const center_angle = CalculateAngle(info);

    float const range_start = center_angle - info.Spread / 2;

    float const offset =
        static_cast<float>(rand() % static_cast<int>(info.Spread * 100)) / 100;

    float const angle = range_start + offset;

    auto* bullet = new Projectile(info.ProjProps, info.ProjSpeed, angle,
                                  info.HitAnimation, info.IsPlayerOwned);

    bullets.push_back(bullet);

    return bullets;
}

std::vector<Projectile*> CreateHelixBullets(RangedAttackInfo info) {
    std::vector<Projectile*> bullets;

    float const angle = CalculateAngle(info);

    auto* bullet1 =
        new HelixBullet(info.ProjProps, info.ProjSpeed, angle,
                        info.HitAnimation, info.IsPlayerOwned, false);
    auto* bullet2 =
        new HelixBullet(info.ProjProps, info.ProjSpeed, angle,
                        info.HitAnimation, info.IsPlayerOwned, true);

    bullets.push_back(bullet1);
    bullets.push_back(bullet2);

    return bullets;
}

std::vector<Projectile*> CreateRotatingBullets(RangedAttackInfo info) {
    std::vector<Projectile*> bullets;

    float const angle = CalculateAngle(info);

    float const bullet_separation = 2 * M_PI / info.ProjCount;
    RotatingBullet* bullet;
    SDL_Log("projspeed: %f", info.ProjSpeed);

    for (float i = 0; i < 2 * M_PI; i += bullet_separation) {

        bullet = new RotatingBullet(info.ProjProps, info.ProjSpeed, angle,
                                    info.HitAnimation, info.IsPlayerOwned, i);

        bullets.push_back(bullet);
    }

    return bullets;
}

std::vector<Projectile*> CreateSpreadBullets(RangedAttackInfo info) {
    std::vector<Projectile*> bullets;

    float const center_angle = CalculateAngle(info);

    float const spread_start = center_angle - info.Spread / 2;
    float const interval = info.Spread / info.ProjCount;

    Projectile* bullet;
    float angle = 0;

    Player* owner = nullptr;

    if (info.IsPlayerOwned) {
        owner = Application::Get()->GetPlayer();
    }

    for (int i = 0; i < info.ProjCount; i++) {
        angle = spread_start + interval * i;

        bullet =
            new Projectile(info.ProjProps, info.ProjSpeed, angle,
                           info.HitAnimation, info.IsPlayerOwned, 10, 0, owner);
        bullets.push_back(bullet);
    }

    return bullets;
}

std::vector<Projectile*> CreateShotgunSpreadBullets(RangedAttackInfo info) {
    std::vector<Projectile*> bullets;

    float center_angle = CalculateAngle(info);
    float const delta_y = info.TargetY - info.Y;
    float const delta_x = info.TargetX - info.X;

    if (center_angle < 0) {
        center_angle += 2 * M_PI;
    } else if (center_angle > 2 * M_PI) {
        center_angle -= 2 * M_PI;
    }

    SDL_Log("target x: %f, target y: %f", info.TargetX, info.TargetY);
    SDL_Log("delta x: %f, delta y: %f", delta_x, delta_y);

    SDL_Log("center angle: %f", center_angle);

    float const spread_start = center_angle - info.Spread / 2;
    float const interval = info.Spread / info.ProjCount;

    Projectile* bullet;
    float angle = 0;

    Player* owner = nullptr;

    if (info.IsPlayerOwned) {
        owner = Application::Get()->GetPlayer();
    }

    for (int i = 0; i < info.ProjCount; i++) {
        angle = spread_start + interval * i;

        bullet =
            new Projectile(info.ProjProps, info.ProjSpeed, angle,
                           info.HitAnimation, info.IsPlayerOwned,
                           10 + owner->GetStats().GetRangedDamage(), 0, owner);
        bullets.push_back(bullet);
    }

    return bullets;
}

std::vector<Projectile*> CreateRingBullets(RangedAttackInfo info) {
    std::vector<Projectile*> bullets;

    const float outer_radius = 40;
    const float inner_radius = 20;

    float angle = 0;

    float const bullet_separation = 2 * M_PI / info.ProjCount;
    Projectile* bullet;
    Vector2D const bullet_pos;

    for (float i = 0; i < 2 * M_PI; i += bullet_separation) {
        angle = i * 180 / M_PI;
        info.ProjProps.Rotation = angle;

        info.ProjProps.DstRect.x = outer_radius * cos(i) + info.X;
        info.ProjProps.DstRect.y = outer_radius * sin(i) + info.Y;

        bullet = new Projectile(info.ProjProps, info.ProjSpeed, i,
                                info.HitAnimation, info.IsPlayerOwned);

        bullets.push_back(bullet);
    }

    for (float i = bullet_separation / 2; i < 2 * M_PI;
         i += bullet_separation) {
        angle = i * 180 / M_PI;
        info.ProjProps.Rotation = angle;

        info.ProjProps.DstRect.x = inner_radius * cos(i) + info.X;
        info.ProjProps.DstRect.y = inner_radius * sin(i) + info.Y;

        bullet = new Projectile(info.ProjProps, info.ProjSpeed, i,
                                info.HitAnimation, info.IsPlayerOwned);

        bullets.push_back(bullet);
    }

    return bullets;
}
