#include "Projectile.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Physics/CollisionHandler.h"
#include <SDL2/SDL.h>

Projectile::Projectile(Properties& props, int speed, float mass, float angle): GameObject(props), m_Speed(speed), m_Mass(mass), m_Angle(angle){
    m_RigidBody = new RigidBody(m_Mass);
    m_Collider = new Collider();
    m_Collider->SetCorrection(0, 0, 10, 10 );
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());   
}

void Projectile::Draw(){
    GameObject::Draw();
}

void Projectile::Update(float dt, const std::vector<GameObject*>& colliders){
    m_RigidBody->UpdateProjectile(dt);
    m_RigidBody->UnSetForce();
    m_RigidBody->ApplyForceX(m_Speed);
    m_RigidBody->ApplyForceY(m_Angle);
    m_Transform->TranslateX(m_RigidBody->Position().X);
    m_Transform->TranslateY(m_RigidBody->Position().Y);
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    CanMoveThrough(colliders);
}

void Projectile::CanMoveThrough(const std::vector<GameObject*>& colliders)
{
    SDL_Log("%zu", colliders.size());
    for (auto collider : colliders)
    {
        if (*m_Transform->X < 0.0f ||
            *m_Transform->Y < 0.0f ||
            *m_Transform->X + this->GetWidth() > SCREEN_WIDTH ||
            *m_Transform->Y + this->GetHeight() > SCREEN_HEIGHT ||
            CollisionHandler::GetInstance()->CheckCollision(m_Collider->Get(), collider->GetCollider()->Get()))
        {
            float deg = m_Angle * 3.14159/180;
            m_Transform->TranslateX(-((m_RigidBody->Velocity().X/2) * cos(deg)));
            m_Transform->TranslateY(-((m_RigidBody->Velocity().Y/2) * sin(deg)));
        }
    }
}

void Projectile::Clean(){
    Renderer::GetInstance()->Clean();
}
