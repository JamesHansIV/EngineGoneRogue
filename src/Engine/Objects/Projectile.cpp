#include "Projectile.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Character.h"
#include <SDL2/SDL.h>

Projectile::Projectile(Properties& props, int speed, float mass, float angle): Collider(props), m_Speed(speed), m_Angle(angle){
    m_RigidBody.SetMass(mass);
    m_CollisionBox.SetCorrection(0, 0, 10, 10 );
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_MarkedForDeletion = false;
}

void Projectile::Draw(){
    GameObject::Draw();
}

void Projectile::Update(float dt, const std::vector<Collider*>& colliders){
    m_RigidBody.UpdateProjectile(dt);
    m_RigidBody.UnSetForce();
    m_RigidBody.ApplyForceX(m_Speed);
    m_RigidBody.ApplyForceY(m_Angle);
    m_Transform->TranslateX(m_RigidBody.Position().X);
    m_Transform->TranslateY(m_RigidBody.Position().Y);
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    CanMoveThrough(colliders);
}

void Projectile::CanMoveThrough(const std::vector<Collider*>& colliders)
{
    if (*m_Transform->X < 0.0f ||
        *m_Transform->Y < 0.0f ||
        *m_Transform->X + this->GetWidth() > SCREEN_WIDTH ||
        *m_Transform->Y + this->GetHeight() > SCREEN_HEIGHT)
    {
        m_MarkedForDeletion = true;
    }
    for (auto& collider : colliders)
    {
        if(ColliderHandler::GetInstance()->CheckCollision(m_CollisionBox.GetRect(), collider->GetCollisionBox().GetRect()))
        {
            if (dynamic_cast<Character*>(collider)) {
                collider->GetHealthObj()->SetDamage(22);
            }
            m_MarkedForDeletion = true;
        }
    }
}

void Projectile::Clean(){
}
