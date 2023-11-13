#include "Player.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Physics/CollisionHandler.h"


Player::Player(Properties& props): Character(props){
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 2, 500);
    m_RigidBody = new RigidBody();
    m_Collider = new Collider();
    // m_Collider->SetCorrection(-45, -20, 60, 80 );
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_Health = new Health(100);
}


void Player::Draw(){
    m_Animation->Draw(m_Transform->GetX(), m_Transform->GetY(), m_DstRect.w, m_DstRect.h);
    DrawPlayerHealth();
}

void Player::DrawPlayerHealth(){
    const int HEALTH_BAR_HEIGHT = 10;

    int healthBarWidth = static_cast<int>((GetWidth() + 15) * (m_Health->GetHealth() / 100.0));

    int healthBarX = GetX();
    int healthBarY = GetY() - HEALTH_BAR_HEIGHT - 5;

    SDL_Rect healthBarRect;
    healthBarRect.x = healthBarX - Renderer::GetInstance()->GetCameraX() - 5;
    healthBarRect.y = healthBarY - Renderer::GetInstance()->GetCameraY();
    healthBarRect.w = healthBarWidth;
    healthBarRect.h = HEALTH_BAR_HEIGHT;
    
    SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 0, 255, 0, 255);
    SDL_RenderFillRect(Renderer::GetInstance()->GetRenderer(), &healthBarRect);
}

void Player::Update(float dt){
    m_RigidBody->Update(dt);
    m_RigidBody->UnSetForce();
    if (InputChecker::IsKeyPressed(SDLK_w)) {
        m_RigidBody->ApplyForceY(-13);
    }
    if (InputChecker::IsKeyPressed(SDLK_s)) {
        m_RigidBody->ApplyForceY(13);
    }
    if (InputChecker::IsKeyPressed(SDLK_a)) {
        m_RigidBody->ApplyForceX(-13);
        // TODO: Add run animation
        //m_Animation->SetProps("player_run", 1, 8, 100, SDL_FLIP_HORIZONTAL);
        //SetFlip(SDL_FLIP_HORIZONTAL);
    }
    if (InputChecker::IsKeyPressed(SDLK_d)) {
        m_RigidBody->ApplyForceX(13);
        //m_Animation->SetProps("player_run", 1, 8, 100);
        //SetFlip(SDL_FLIP_NONE);
    }
    m_Transform->Translate(m_RigidBody->Position());
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_Animation->Update();
    CanMoveThrough();
}

void Player::CanMoveThrough()
{
    for (auto *collider : m_Colliders)
    {
        if (*m_Transform->X < 0.0F ||
            *m_Transform->Y < 0.0F ||
            *m_Transform->X + this->GetWidth() > SCREEN_WIDTH ||
            *m_Transform->Y + this->GetHeight() > SCREEN_HEIGHT ||
            CollisionHandler::GetInstance()->CheckCollision(m_Collider->Get(), collider->GetCollider()->Get()))
        {
            m_Transform->TranslateX(-m_RigidBody->Velocity().X/2);
            m_Transform->TranslateY(-m_RigidBody->Velocity().Y/2);
            m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());            
        }
    }
}

void Player::OnEvent(Event& event) {
    SDL_Event const e = event.GetEvent();
    if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_a) {
            // dont think this is needed anymore, character does not face right or left
            //m_Animation->SetProps("player", 1, 6, 100, SDL_FLIP_HORIZONTAL);
            //SetFlip(SDL_FLIP_HORIZONTAL);
        }
        if (e.key.keysym.sym == SDLK_d) {
            //m_Animation->SetProps("player", 1, 6, 100);
            //SetFlip(SDL_FLIP_NONE);
        }
    }
}

void Player::Clean(){
    Renderer::GetInstance()->Clean();
}

