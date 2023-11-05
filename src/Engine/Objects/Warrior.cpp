#include "Warrior.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/InputChecker.h"
#include "Engine/Components/CollisionHandler.h"

Warrior::Warrior(Properties& props): Character(props){
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 6, 80);
    m_RigidBody = new RigidBody();
    m_Collider = new Collider();
    m_Collider->SetCorrection(100, 100, 60, 70 );
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
}

void Warrior::Draw(){
    m_Animation->Draw(m_Transform->GetX(), m_Transform->GetY(), m_DstRect.w, m_DstRect.h);
}

void Warrior::Update(float dt, const std::vector<GameObject*>& colliders){
    m_RigidBody->Update(dt);
    m_RigidBody->UnSetForce();
    bool canMove = !canMoveThrough(colliders);
    if(canMove)
    {
        if (InputChecker::isKeyPressed(SDLK_w)) {
            m_RigidBody->ApplyForceY(-20);
        }
        if (InputChecker::isKeyPressed(SDLK_s)) {
            m_RigidBody->ApplyForceY(20);
        }
        if (InputChecker::isKeyPressed(SDLK_a)) {
            m_RigidBody->ApplyForceX(-20);
            m_Animation->SetProps("player_run", 1, 8, 100, SDL_FLIP_HORIZONTAL);
            setFlip(SDL_FLIP_HORIZONTAL);
        }
        if (InputChecker::isKeyPressed(SDLK_d)) {
            m_RigidBody->ApplyForceX(20);
            m_Animation->SetProps("player_run", 1, 8, 100);
            setFlip(SDL_FLIP_NONE);
        } 
    }
    m_RigidBody->Update(dt);
    m_Transform->Translate(m_RigidBody->Position());
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_Animation->Update();
}

bool Warrior::canMoveThrough(const std::vector<GameObject*>& colliders)
{
    for (auto collider : colliders) {
        auto coll = collider->getCollider()->Get();
        auto coll2 = m_Collider->Get();
        if (CollisionHandler::GetInstance()->CheckCollision(m_Collider->Get(), collider->getCollider()->Get())) {
            m_Transform->TranslateX(-m_RigidBody->Velocity().X);
            m_Transform->TranslateY(-m_RigidBody->Velocity().Y);
            m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
            return true;
        }
    }
    return false;
}

void Warrior::OnEvent(Event& event) {
    SDL_Event e = event.getEvent();
    if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_a) {
            m_Animation->SetProps("player", 1, 6, 100, SDL_FLIP_HORIZONTAL);
            setFlip(SDL_FLIP_HORIZONTAL);
        }
        if (e.key.keysym.sym == SDLK_d) {
            m_Animation->SetProps("player", 1, 6, 100);
            setFlip(SDL_FLIP_NONE);
        }
    }
}

void Warrior::Clean(){
    Renderer::GetInstance()->Clean();
}

