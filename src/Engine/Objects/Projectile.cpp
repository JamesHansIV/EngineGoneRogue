#include "Projectile.h"
#include "Engine/Renderer/Renderer.h"

Projectile::Projectile(Properties& props): GameObject(props){
    m_RigidBody = new RigidBody();
}

void Projectile::Draw(){
    GameObject::Draw();
}

void Projectile::Update(float dt){
    m_RigidBody->Update(0.3);
    m_RigidBody->ApplyForceX(50);
    m_Transform->TranslateX(m_RigidBody->Position().X);
}

void Projectile::Clean(){
    Renderer::GetInstance()->Clean();
}
