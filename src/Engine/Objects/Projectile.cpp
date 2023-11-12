#include "Projectile.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Physics/CollisionHandler.h"
#include <SDL2/SDL.h>

Projectile::Projectile(Properties& props, int speed, float mass, float angle): GameObject(props), m_Speed(speed), m_Mass(mass), m_Angle(angle){
    m_RigidBody = new RigidBody(m_Mass);
    m_Collider = new Collider();
    m_Collider->SetCorrection(0, 0, 10, 10 );
    // m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());   
}

void Projectile::Draw(){
    // m_Animation->Draw(this->GetX(), this->GetY(), m_DstRect.w, m_DstRect.h);
    // Renderer::GetInstance()::Draw(GetTextureID(), m_Transform->X, m_Transform->Y, GetHeight(), GetWidth());
    GameObject::Draw();
}

void Projectile::Update(float dt, const std::vector<GameObject*>& colliders){
    m_RigidBody->UpdateProjectile(dt);
    m_RigidBody->UnSetForce();
    m_RigidBody->ApplyForceX(m_Speed);
    m_RigidBody->ApplyForceY(m_Angle);
    // SDL_Log("%s, %f  ,%f", "Position beforeee: ", *m_Transform->X,  *m_Transform->Y);
    m_Transform->TranslateX(m_RigidBody->Position().X);
    m_Transform->TranslateY(m_RigidBody->Position().Y);
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    canMoveThrough(colliders);
}

bool Projectile::canMoveThrough(const std::vector<GameObject*>& colliders)
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
            SDL_Log("%s, %f  ,%f", "Position before: ", *m_Transform->X,  *m_Transform->Y);
            SDL_Log("%s, %f, %f", "Velocity: ", m_RigidBody->Velocity().X, m_RigidBody->Velocity().Y );
            float deg = m_Angle * 3.14159/180;
            m_Transform->TranslateX(-((m_RigidBody->Velocity().X/2) * cos(deg)));
            m_Transform->TranslateY(-((m_RigidBody->Velocity().Y/2) * sin(deg)));
            SDL_Log("%s, %f", "Angle: ", m_Angle);
            SDL_Log("%s, %f  ,%f", "Position after: ", *m_Transform->X,  *m_Transform->Y);
            // m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
            // exit(1);
            // SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 255, 0, 0, 255); // Set red color (adjust as needed)
            // // Render the collider with the changed colors
            // SDL_Rect rect = collider->getCollider()->Get();

            // SDL_RenderDrawRect(Renderer::GetInstance()->GetRenderer(), &rect);
            // SDL_RenderPresent(Renderer::GetInstance()->GetRenderer());
        }
        

    }
    return false;
}

void Projectile::Clean(){
    Renderer::GetInstance()->Clean();
}
