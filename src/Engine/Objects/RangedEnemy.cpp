#include "RangedEnemy.h"

RangedEnemy::RangedEnemy(Properties& props, int perceptionWidth, int perceptionHeight)
    : Enemy(props, perceptionWidth, perceptionHeight) {}


void RangedEnemy::Draw() {
    for (auto* projectile : m_Projectiles) {
        projectile->Draw();
    }
    
}

void RangedEnemy::Update(float dt) {

}

void RangedEnemy::Shoot() {
    float target_x = GetTarget()->GetMidPointX(); 
    float target_y = GetTarget()->GetMidPointY(); 
    float delta_y = target_y - GetY();
    float delta_x = target_x - GetX();

    float angle = atan2(delta_y, delta_x) * (180.0 / M_PI);

    Properties props = {
        "../assets/textures/spritesheets/enemies.png",
        { 0, 0, 16, 16 },
        { target_x, target_y, 10, 10 },
        angle};

    Projectile* bullet = new Projectile(props, 3, angle);
    m_Projectiles.push_back(bullet);
}

void RangedEnemy::OnCollide(Collider* collidee) {

}

void RangedEnemy::Clean() {
    
}