#include "Player.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Physics/CollisionHandler.h"

Player::Player(Properties& props): Character(props){
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 6, 80);
    m_RigidBody = new RigidBody();
    m_Collider = new Collider();
    m_Collider->SetCorrection(100, 100, 60, 70 );
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
}

void Player::Draw(){
    m_Animation->Draw(m_Transform->GetX(), m_Transform->GetY(), m_DstRect.w, m_DstRect.h);
}

void Player::Update(float dt, const std::vector<GameObject*>& colliders){
    m_RigidBody->Update(dt);
    m_RigidBody->UnSetForce();
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
    m_Transform->Translate(m_RigidBody->Position());
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_Animation->Update();
    canMoveThrough(colliders);
}

bool Player::canMoveThrough(const std::vector<GameObject*>& colliders)
{
    for (auto collider : colliders)
    {
        if (*m_Transform->X < 0.0f ||
            *m_Transform->Y < 0.0f ||
            *m_Transform->X + this->GetWidth() > SCREEN_WIDTH ||
            *m_Transform->Y + this->GetHeight() > SCREEN_HEIGHT ||
            CollisionHandler::GetInstance()->CheckCollision(m_Collider->Get(), collider->getCollider()->Get()))
        {
            m_Transform->TranslateX(-m_RigidBody->Velocity().X/2);
            m_Transform->TranslateY(-m_RigidBody->Velocity().Y/2);
            m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
            return true;
        }
    }
    return false;
}

void Player::OnEvent(Event& event) {
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

void Player::Clean(){
    Renderer::GetInstance()->Clean();
}

