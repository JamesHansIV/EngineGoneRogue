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
    m_Collider->Set(m_Transform->X, m_Transform->Y, GetHeight(), GetWidth());
}

void Warrior::Draw(){
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
}

void Warrior::Update(float dt, const std::vector<GameObject*>& colliders){
    m_RigidBody->Update(dt);
    m_RigidBody->UnSetForce();
    bool canMove = !canMoveThrough(colliders);
    SDL_Log("%s", canMove?"true":"false");
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
    m_Collider->Set(m_Transform->X, m_Transform->Y, GetHeight(), GetWidth());
    m_Animation->Update();
}

bool Warrior::canMoveThrough(const std::vector<GameObject*>& colliders)
{
    for (auto collider : colliders) {
        auto coll = collider->getCollider()->Get();
        auto coll2 = m_Collider->Get();
        SDL_Log("SDL_Rect values: x=%d, y=%d, width=%d, height=%d", coll.x, coll.y, coll.w, coll.h);
        SDL_Log("SDL_Rect values: x=%d, y=%d, width=%d, height=%d", coll2.x, coll2.y, coll2.w, coll2.h);
        if (CollisionHandler::GetInstance()->CheckCollision(m_Collider->Get(), collider->getCollider()->Get())) {
            SDL_Log("%s, %f %f", "Velocity", m_RigidBody->Velocity().X, m_RigidBody->Velocity().Y );
            SDL_Log("%s, %f %f", "Position before", this->GetX(), this->GetY() );
            m_Transform->TranslateX(-m_RigidBody->Velocity().X);
            m_Transform->TranslateY(-m_RigidBody->Velocity().Y);
            m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
            SDL_Log("%s, %f %f", "Position after", this->GetX(), this->GetY() );
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

