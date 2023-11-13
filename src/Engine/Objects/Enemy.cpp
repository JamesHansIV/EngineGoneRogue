#include "Enemy.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Physics/CollisionHandler.h"


Enemy::Enemy(Properties& props): Character(props){
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 2, 500);
    m_RigidBody = new RigidBody();
    m_Collider = new Collider();
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_Health = new Health(100);
    m_MarkedForDeletion = false;
}


void Enemy::Draw(){
    m_Animation->Draw(m_Transform->GetX(), m_Transform->GetY(), m_DstRect.w, m_DstRect.h);
    if (m_MarkedForDeletion == false)
        DrawEnemyHealth();
}

void Enemy::DrawEnemyHealth(){
    const int HEALTH_BAR_HEIGHT = 10;

    int healthBarWidth = static_cast<int>((GetWidth() + 15) * (m_Health->GetHealth() / 100.0));
    if (healthBarWidth <= 0) healthBarWidth = 0;
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

void Enemy::Update(float dt){
    if(m_Health->GetHealth() <= 0)
    {
        m_Animation->SetProps("player_dead", 1, 6, 500);
        if (m_Animation->GetCurrentFrame() == 6-1) {
            m_MarkedForDeletion = true;
        }
    }
    m_Animation->Update();
}

void Enemy::OnEvent(Event& event) {
}

void Enemy::Clean(){
    delete m_Animation;
    delete m_RigidBody;
}

