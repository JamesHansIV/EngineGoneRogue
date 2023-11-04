#include "Projectile.h"
#include "Engine/Renderer/Renderer.h"

Projectile::Projectile(Properties& props, int speed, float mass, float angle): GameObject(props), m_Speed(speed), m_Mass(mass), m_Angle(angle){
    m_RigidBody = new RigidBody(m_Mass);
}

void Projectile::Draw(){
    GameObject::Draw();
}

void Projectile::Update(float dt){
    m_RigidBody->UpdateProjectile(dt);
    m_RigidBody->ApplyForceX(m_Speed);
    m_RigidBody->ApplyForceY(m_Angle);

    m_Transform->TranslateX(m_RigidBody->Position().X);
    m_Transform->TranslateY(m_RigidBody->Position().Y);
}

void Projectile::Clean(){
    Renderer::GetInstance()->Clean();
}
